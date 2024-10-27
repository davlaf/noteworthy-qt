#include "ClientWebSocketHandler.hpp"
#include "RoomState.hpp"

#include "Stroke.hpp"
#include "nlohmann/json.hpp"
#include <qwebsockethandshakeoptions.h>

ClientWebSocketHandler::ClientWebSocketHandler(QObject *parent)
    : QObject{parent}
{
    connect(&webSocket, &QWebSocket::connected, this, &ClientWebSocketHandler::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &ClientWebSocketHandler::closed);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred),
            this, &ClientWebSocketHandler::onError); // Handle WebSocket errors
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

void ClientWebSocketHandler::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "WebSocket error occurred:" << webSocket.errorString();
    qDebug() << "Error code:" << error;
}

void ClientWebSocketHandler::onTextMessageReceived(QString message) {
    nlohmann::json event = nlohmann::json::parse(message.toStdString());
    handleEvent(message.toStdString());
}

void ClientWebSocketHandler::sendEvent(const nlohmann::json& event) {
    webSocket.sendTextMessage(QString::fromStdString(event.dump()));
}

std::unique_ptr<CanvasObject> createObject(CanvasObject::ObjectType object_type) {
    switch (object_type) {
    case CanvasObject::STROKE: {
        // Initialize current_path with a new QPainterPath

        auto path = std::make_unique<QPainterPath>();

        // Create a Stroke using the current_path
        auto stroke = std::make_unique<Stroke>(std::move(path));
        return std::move(stroke);
    }
    case CanvasObject::SYMBOL:
        qDebug() << "Symbol creation not supported.";
        break;
    case CanvasObject::SHAPE:
        qDebug() << "Shape creation not supported.";
        break;
    case CanvasObject::TEXT:
        qDebug() << "Text creation not supported.";
        break;
    case CanvasObject::BACKGROUND_IMAGE:
        qDebug() << "Background image creation not supported.";
        break;
    default:
        qDebug() << "Unsupported object type!";
    }
    assert(false); // Unsupported object
    return nullptr;
}

void ClientWebSocketHandler::handleEvent(const nlohmann::json& event) {
    assert(event["room_id"] == state.room_id);

    auto event_type = static_cast<CanvasObject::EventType>(event["event_type"]);
    switch(event_type) {
    case CanvasObject::CREATE: {
        state.manipulatePage(event["page_id"], [event](Page& page) mutable {
            auto object_type = static_cast<CanvasObject::ObjectType>(event["object_type"]);

            std::unique_ptr<CanvasObject> object = createObject(object_type);
            object->fromJson(event);
            page.addObject(std::move(object));
        });
        break;
    }
    case CanvasObject::DELETE: {
        uint64_t object_id = event["object_id"];
        state.manipulatePage(event["page_id"], [object_id](Page& page){
            page.deleteObject(object_id);
        });
        break;
    }
    case CanvasObject::MOVE:
    case CanvasObject::SCALE:
    case CanvasObject::ROTATE:
    case CanvasObject::APPEND:
    case CanvasObject::EDIT: {
        uint64_t object_id = event["object_id"];
        state.manipulatePage(event["page_id"], [object_id, event](Page& page){
            page.manipulateObject(object_id, [event](CanvasObject& canvas_object){
                canvas_object.applyEvent(event);
            });
        });
        break;
    }
    default: {
        qDebug() << "event type not recognized in clientwebsockethandler";
        assert(false);
        break;
    }
    }
}
