#pragma once

#include "CanvasObject.hpp"
#include "nlohmann/json_fwd.hpp"
#include <QObject>
#include <qgraphicsscene.h>
#include <qwebsocket.h>

class ClientWebSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientWebSocketHandler(std::shared_ptr<QGraphicsScene> scene, QObject *parent = nullptr);
    void handleEvent(const nlohmann::json& event);
    void sendEvent(const nlohmann::json& event);
    std::unique_ptr<CanvasObject> createObject(CanvasObject::ObjectType object_type);

signals:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onError(QAbstractSocket::SocketError error);

private:
    QWebSocket webSocket;
    std::shared_ptr<QGraphicsScene> scene;
};
