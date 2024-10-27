#pragma once

#include "nlohmann/json_fwd.hpp"
#include <QObject>
#include <qwebsocket.h>

class ClientWebSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientWebSocketHandler(QObject *parent = nullptr);
    void handleEvent(const nlohmann::json& event);
    void sendEvent(const nlohmann::json& event);

signals:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onError(QAbstractSocket::SocketError error);

private:
    QWebSocket webSocket;
};
