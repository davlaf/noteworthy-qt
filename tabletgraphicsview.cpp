#include "tabletgraphicsview.h"
#include <QWebSocketHandshakeOptions>

#include "nlohmann/json.hpp"
#include "Stroke.hpp"

using json = nlohmann::json; // Define a shorthand for the json type

TabletGraphicsView::TabletGraphicsView(QWidget *parent): QGraphicsView(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents);  // Enable touch events
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // i dont know why I have to subtract 5
    this->setSceneRect(0, 0, this->width()-5, this->height()-5);

    // websocket
    qDebug() << "websocket??";
    connect(&webSocket, &QWebSocket::connected, this, &TabletGraphicsView::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &TabletGraphicsView::closed);
    QWebSocketHandshakeOptions options;
    options.setSubprotocols({"echo-protocol"});
    webSocket.open(QUrl("ws://127.0.0.1:8081/"), options);

    this->setScene(&scene);
    this->pen = QColor{255, 50, 50};
    this->show();
}

void TabletGraphicsView::handleTouch(QPointF position, int id)
{
    // qDebug() << "Touch pressed at:" << position << "with ID:" << id;
    // only care about 1 touch
    if (id != 0) {
        return;
    }
    QPointF scenePos = this->mapToScene(position.toPoint());
    currentPath = new QPainterPath(scenePos);
    currentPathItem = this->scene.addPath(*currentPath, pen);
}

void TabletGraphicsView::handleMove(QPointF position, int id)
{
    // qDebug() << "Touch moved to:" << position << "with ID:" << id;
    // only care about 1 touch
    if (id != 0) {
        return;
    }

    QPointF scenePos = this->mapToScene(position.toPoint());

    if (currentPath && currentPathItem) {
        // Modify the existing path
        currentPath->lineTo(scenePos);  // Extend the path to the new point
        currentPathItem->setPath(*currentPath);  // Update the QGraphicsPathItem
    }
    // this->scene.removeItem(graphicsPathItem);
}

void TabletGraphicsView::handleRelease(QPointF position, int id)
{

    // qDebug() << "Touch released at:" << position << "with ID:" << id;
    // only care about 1 touch
    if (id != 0) {
        return;
    }

    QPointF scenePos = this->mapToScene(position.toPoint());

    if (currentPath && currentPathItem) {
        // Finalize the path upon release
        currentPath->lineTo(scenePos);  // Ensure the path ends at the final touch point
        Stroke finalStroke;
        finalStroke.fromQPainterPath(*currentPath);
        json pointJson;
        finalStroke.to_json(pointJson, finalStroke);
        const int NEW_STROKE = 0;
        pointJson["type"] = NEW_STROKE;

        QString json_string = QString::fromStdString(pointJson.dump());
        webSocket.sendTextMessage(json_string);
        qDebug() << json_string;

        currentPathItem->setPath(*currentPath);  // Update the QGraphicsPathItem
        currentPath = nullptr;  // Reset the path for the next touch
        currentPathItem = nullptr;  // Reset the item for the next touch


    }
}

//! [onConnected]
void TabletGraphicsView::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &TabletGraphicsView::onTextMessageReceived);
    // webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}
//! [onConnected]

//! [onTextMessageReceived]
void TabletGraphicsView::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
    // Convert QString to std::string
    std::string stdMessage = message.toStdString();

    try {
        // Parse the std::string into a nlohmann::json object
        nlohmann::json event_map = nlohmann::json::parse(stdMessage);

        Stroke stroke;
        stroke.from_json(event_map, stroke);

        QPainterPath* newPath = stroke.toQPainterPath();

        this->scene.addPath(*newPath, pen);
        // qDebug() << "Parsed JSON:" << QString::fromStdString(event_map.dump());

    } catch (nlohmann::json::parse_error& e) {
        qDebug() << "JSON parse error:" << e.what();
    }
}
//! [onTextMessageReceived]

void TabletGraphicsView::resizeEvent(QResizeEvent *event) {
    // Update the scene rectangle to match the new size of the view
    QGraphicsView::resizeEvent(event); // Call the base class implementation (optional if you don't want any default behavior)
    this->setSceneRect(0, 0, this->width()-5, this->height()-5);
}
