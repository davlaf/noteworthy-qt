#include "ClientWebSocketHandler.hpp"
#include "RoomState.hpp"

#include "Stroke.hpp"
#include <qgraphicsitem.h>
#include <qwebsockethandshakeoptions.h>

ClientWebSocketHandler::ClientWebSocketHandler(QObject *parent)
    : QObject{parent}
{
    connect(&webSocket, &QWebSocket::connected, this, &ClientWebSocketHandler::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &ClientWebSocketHandler::closed);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred),
            this, &ClientWebSocketHandler::onError); // Handle WebSocket errors
}

void ClientWebSocketHandler::openConnection()
{
    QWebSocketHandshakeOptions options;
    options.setSubprotocols({"echo-protocol"});
    // webSocket.open(QUrl("wss://nw-ws.howdoesthiseven.work/"), options);
    webSocket.open(QUrl("ws://localhost:8081/"), options);
}

void ClientWebSocketHandler::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &ClientWebSocketHandler::onTextMessageReceived);
}

void ClientWebSocketHandler::closed()
{
    qDebug() << "closed!!";
}

void ClientWebSocketHandler::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "WebSocket error occurred:" << webSocket.errorString();
    qDebug() << "Error code:" << error;
}

void ClientWebSocketHandler::onTextMessageReceived(QString message)
{
    try {
        nlohmann::json event = nlohmann::json::parse(message.toStdString());
        handleEvent(event);
    } catch (...) {
        qDebug() << "error handling!!!!";
    }

}

void ClientWebSocketHandler::sendEvent(const nlohmann::json &event)
{
    webSocket.sendTextMessage(QString::fromStdString(event.dump()));
}

std::unique_ptr<CanvasObject> ClientWebSocketHandler::createCanvasObject(EventObjectType object_type, QGraphicsScene &scene, QColor color)
{
    switch (object_type)
    {
    case STROKE:
    {
        // Initialize current_path with a new QPainterPath

        auto path = QPainterPath();
        qDebug() << "addedPath: " << path;
        QGraphicsPathItem *item = scene.addPath(path, color);
        // Create a Stroke using the current_path
        auto stroke = std::make_unique<Stroke>(path, item);
        qDebug() << "created object:" << stroke.get();
        return std::move(stroke);
    }
    case SYMBOL:
        qDebug() << "Symbol creation not supported.";
        break;
    case SHAPE:
        qDebug() << "Shape creation not supported.";
        break;
    case TEXT:
        qDebug() << "Text creation not supported.";
        break;
    case BACKGROUND_IMAGE:
        qDebug() << "Background image creation not supported.";
        break;
    default:
        qDebug() << "Unsupported object type!";
    }
    assert(false); // Unsupported object
    return nullptr;
}

std::vector<QColor> colors = {
    QColor{255, 0, 0},
    QColor{255, 127, 0},
    // QColor{255, 255, 0}, remove yellow
    QColor{0, 255, 0},
    QColor{0, 0, 255},
    QColor{75, 0, 130},
    QColor{148, 0, 211},
};

QColor stringToColor(const std::string &string)
{
    int sum = 0;
    for (char c : string)
        sum += c;

    return colors[sum % colors.size()];
}

void ClientWebSocketHandler::handleEvent(const nlohmann::json &event)
{
    if (event["room_id"] != state.room_id) {
        return;
    } // ignore events from other rooms
    // TODO: make this on the server side instead

    auto event_type = static_cast<EventType>(event["event_type"]);
    switch (event_type)
    {
    case CREATE:
    {
        auto object_type = static_cast<EventObjectType>(event["object_type"]);
        switch (object_type)
        {

        case ROOM:
        {
            // replace everything
            state.fromJson(event);
            break;
        }
        case PAGE:
        {
            state.applyInsertPageEvent(event);
            emit pageCreated(event["page_id"]);
            break;
        }

        case STROKE:
        case SYMBOL:
        case SHAPE:
        case TEXT:
        case BACKGROUND_IMAGE:
            state.manipulatePage(event["page_id"], [this, event, object_type](Page &page) mutable
                                 {
                std::unique_ptr<CanvasObject> object =
                    createCanvasObject(object_type, *page.scene, stringToColor(event["owner_id"]));
                object->fromJson(event);
                page.addObject(std::move(object)); });
            break;
        default:
            qDebug("fake object type");
            assert(false);
        }
        break;
    }
    case DELETE:
    {
        auto object_type = static_cast<EventObjectType>(event["object_type"]);
        switch (object_type)
        {
        case ROOM:
        {
            // replace everything
            qDebug() << "can't delete a room I dont think???";
            assert(false);
            break;
        }
        case PAGE:
        {
            emit pageDeleted(event["page_id"]);
            state.applyDeletePageEvent(event);

            break;
        }
        case STROKE:
        case SYMBOL:
        case SHAPE:
        case TEXT:
        case BACKGROUND_IMAGE:
        {
            uint64_t object_id = event["object_id"];
            state.manipulatePage(event["page_id"], [object_id](Page &page)
                                 {
                page.manipulateObject(object_id, [](CanvasObject& object){
                    QGraphicsScene* scene = object.item->scene();
                    if (scene) {
                        scene->removeItem(object.item);
                    }

                    delete object.item;
                    object.item = nullptr;
                });
                page.deleteObject(object_id);
            });
            break;
        }
        default:
            qDebug("fake object type");
            assert(false);
        }
        break;
    }
    case MOVE:
    case SCALE:
    case ROTATE:
    case APPEND:
    case EDIT:
    {
        // assume its an object
        uint64_t object_id = event["object_id"];
        state.manipulatePage(event["page_id"], [object_id, event](Page &page)
                             { page.manipulateObject(object_id, [event](CanvasObject &canvas_object)
                                                     { canvas_object.applyEvent(event); }); });
        break;
    }
    default:
    {
        qDebug() << "event type not recognized in clientwebsockethandler";
        assert(false);
        break;
    }
    }
}
