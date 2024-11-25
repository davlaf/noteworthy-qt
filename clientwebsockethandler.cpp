#include "ClientWebSocketHandler.hpp"
#include "RoomState.hpp"

#include "Stroke.hpp"
#include <qgraphicsitem.h>
#include <qwebsockethandshakeoptions.h>

ClientWebSocketHandler::ClientWebSocketHandler(QObject* parent)
    : QObject { parent }
{
    connect(&webSocket, &QWebSocket::connected, this, &ClientWebSocketHandler::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &ClientWebSocketHandler::closed);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred),
        this, &ClientWebSocketHandler::onError); // Handle WebSocket errors
}

void ClientWebSocketHandler::openConnection(std::string username, std::string room_id)
{
    QWebSocketHandshakeOptions options;
    options.setSubprotocols({ "echo-protocol" });
    std::string url_string = "ws://localhost:8081?username="+username+"&room_id="+room_id;
    // std::string url_string = "wss://nw-ws.howdoesthiseven.work?username="+username+"&room_id="+room_id;
    webSocket.open(QUrl::fromUserInput(QString::fromStdString(url_string)), options);
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

void ClientWebSocketHandler::sendEvent(const nlohmann::json& event)
{
    webSocket.sendTextMessage(QString::fromStdString(event.dump()));
}

std::unique_ptr<CanvasObject> ClientWebSocketHandler::createCanvasObject(EventObjectType object_type, QGraphicsScene& scene, QColor color)
{
    switch (object_type) {
    case STROKE: {
        // Initialize current_path with a new QPainterPath
        auto path = QPainterPath();
        qDebug() << "addedPath: " << path;
        QGraphicsPathItem* item = scene.addPath(path, color);
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
    default:
        qDebug() << "Unsupported object type!";
    }
    assert(false); // Unsupported object
    return nullptr;
}

std::vector<QColor> colors = {
    QColor { 255, 0, 0 },
    QColor { 255, 127, 0 },
    // QColor{255, 255, 0}, remove yellow
    QColor { 0, 255, 0 },
    QColor { 0, 0, 255 },
    QColor { 75, 0, 130 },
    QColor { 148, 0, 211 },
};

QColor stringToColor(const std::string& string)
{
    int sum = 0;
    for (char c : string)
        sum += c;

    return colors[sum % colors.size()];
}

void ClientWebSocketHandler::handleEvent(const nlohmann::json& event)
{
    if (event["room_id"] != state.room_id) {
        return;
    } // ignore events from other rooms

    auto object_type = static_cast<EventObjectType>(event["object_type"]);
    switch (object_type) {
    case ROOM: {
        auto event_type = static_cast<RoomState::RoomEventType>(event["event_type"]);
        switch (event_type) {
        case RoomState::RoomEventType::DELETE: {
            throw std::runtime_error("shouldn't be able to delete room");
            break;
        }
        default: {
            state.applyEvent(event);
            break;
        }
        }
        break;
    }
    case PAGE: {
        auto event_type = static_cast<Page::PageEventType>(event["event_type"]);
        switch (event_type) {
        case Page::PageEventType::CREATE: {
            throw std::runtime_error("can only insert page not create");
            break;
        }
        case Page::PageEventType::DELETE: {
            emit pageDeleted(event["page_id"]);
            state.deletePage(event["page_id"]);
            break;
        }
        case Page::PageEventType::INSERT: {
            state.applyInsertPageEvent(event);
            emit pageCreated(event["page_id"]);
            break;
        }
        default: {
            state.manipulatePage(event["page_id"], [&](Page& page) {
                page.applyEvent(event);
            });
        }
        }

        break;
    }
    case USER: {
        auto event_type = static_cast<User::UserEventType>(event["event_type"]);
        switch (event_type) {
        case User::UserEventType::CREATE: {
            auto user = std::make_unique<User>();
            user->fromJson(event);
            state.addUser(std::move(user));
            break;
        }
        case User::UserEventType::DELETE: {
            throw std::logic_error("shouldn't ever delete user");
            state.removeUser(event["username"]);
            break;
        }
        default:
            state.manipulateUser(event["username"], [&](User& user) {
                user.applyEvent(event);
            });
            break;
        }
        break;
    }
    case STROKE:
    case SYMBOL:
    case SHAPE:
    case TEXT: {
        auto event_type = static_cast<CanvasObject::CanvasObjectEventType>(event["event_type"]);
        switch (event_type) {
        case CanvasObject::CanvasObjectEventType::CREATE: {
            state.manipulatePage(event["page_id"], [this, event, object_type](Page& page) mutable {
                std::unique_ptr<CanvasObject> object = createCanvasObject(object_type, *page.scene, stringToColor(event["owner_id"]));
                object->fromJson(event);
                page.addObject(std::move(object));
            });
            break;
        }
        case CanvasObject::CanvasObjectEventType::DELETE: {
            uint64_t object_id = event["object_id"];
            state.manipulatePage(event["page_id"], [object_id](Page& page) {
                page.manipulateObject(object_id, [](CanvasObject& object) {
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
        default: {
            uint64_t object_id = event["object_id"];
            state.manipulatePage(event["page_id"], [object_id, event](Page& page) {
                page.manipulateObject(object_id, [event](CanvasObject& canvas_object) {
                    canvas_object.applyEvent(event);
                });
            });
            break;
        }
        }
        break;
    }
    }
}
