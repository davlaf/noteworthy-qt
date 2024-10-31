#include "tabletgraphicsview.h"
#include "RoomState.hpp"
#include <QWebSocketHandshakeOptions>

#include "nlohmann/json.hpp"
#include "Stroke.hpp"

using json = nlohmann::json; // Define a shorthand for the json type

TabletGraphicsView::TabletGraphicsView(QWidget *parent) : QGraphicsView(parent), web_socket_handler(scene)
{
    setAttribute(Qt::WA_AcceptTouchEvents); // Enable touch events
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // i dont know why I have to subtract 5
    this->setSceneRect(0, 0, this->width() - 5, this->height() - 5);
    qDebug() << "v0.1";

    this->setScene(scene.get());
    this->pen = QColor{255, 50, 50};
    this->show();
}

void TabletGraphicsView::handleTouch(QPointF position, int id)
{
    qDebug() << "Touch pressed at:" << position << "with ID:" << id;
    // only care about 1 touch
    currentTouchId = id;
    QPointF scenePos = this->mapToScene(position.toPoint());

    // Initialize current_path with a new QPainterPath
    auto current_path = QPainterPath(scenePos);

    switch(current_transform){
        case(CanvasObject::CREATE):
        {
            switch(current_object){
                case(CanvasObject::STROKE):
                {
                    // Create a Stroke using the current_path
                    current_stroke = std::make_unique<Stroke>(current_path);
                    current_stroke->room_id = state.room_id;
                    current_stroke->page_id = selected_page_id;
                    current_stroke->object_id = IDGenerator::newID();

                    nlohmann::json event;
                    current_stroke->createCreateEvent(event);
                    web_socket_handler.sendEvent(event);
                    web_socket_handler.handleEvent(event);
                }
                case(CanvasObject::SHAPE):
                {

                }
                case(CanvasObject::TEXT):
                {

                }
                case(CanvasObject::SYMBOL):
                {

                }
                case(CanvasObject::BACKGROUND_IMAGE):
                {

                }
                default:{

                }
            }
        }
        case CanvasObject::DELETE:
        {

        }
        case CanvasObject::MOVE:
        {

        }
        case CanvasObject::SCALE:
        {

        }
        case CanvasObject::ROTATE:
        {

        }
        case CanvasObject::APPEND:
        {

        }
        case CanvasObject::EDIT:
        {

        }
    }
}

void TabletGraphicsView::handleMove(QPointF position, int id)
{
    qDebug() << "Touch moved to:" << position << "with ID:" << id;
    // only care about 1 touch
    if (id != currentTouchId)
    {
        return;
    }

    QPointF scene_pos = this->mapToScene(position.toPoint());

    if (!current_stroke)
    {
        qDebug("fake!!!! move before new touch");
        return;
    }

    current_stroke->path.lineTo(scene_pos); // Extend the path to the new point
    nlohmann::json event_json;
    current_stroke->createAppendEvent(event_json, {{scene_pos.x(), scene_pos.y()}}); // Update the QGraphicsPathItem
    current_stroke->applyAppendEvent(event_json);

    std::string event_string = event_json.dump();
    web_socket_handler.sendEvent(event_json);
    web_socket_handler.handleEvent(event_json);
}

void TabletGraphicsView::handleRelease(QPointF position, int id)
{

    qDebug() << "Touch released at:" << position << "with ID:" << id;
    // only care about 1 touch
    if (id != currentTouchId)
    {
        return;
    }

    QPointF scene_pos = this->mapToScene(position.toPoint());

    if (!(current_stroke))
    {
        qDebug() << "what?? release when there the stroke is null";
        return;
    }

    current_stroke->path.lineTo(scene_pos); // Extend the path to the new point
    nlohmann::json event_json;
    current_stroke->createAppendEvent(event_json, {{scene_pos.x(), scene_pos.y()}}); // Update the QGraphicsPathItem
    current_stroke->applyAppendEvent(event_json);

    web_socket_handler.sendEvent(event_json);
    web_socket_handler.handleEvent(event_json);

    current_stroke = nullptr;
    current_stroke_id = 0;
}

void TabletGraphicsView::resizeEvent(QResizeEvent *event)
{
    // Update the scene rectangle to match the new size of the view
    QGraphicsView::resizeEvent(event); // Call the base class implementation (optional if you don't want any default behavior)
    this->setSceneRect(0, 0, this->width() - 5, this->height() - 5);
}
