#include "tabletgraphicsview.hpp"
#include "RoomState.hpp"
#include <QWebSocketHandshakeOptions>

#include "Stroke.hpp"

using json = nlohmann::json; // Define a shorthand for the json type

void TabletGraphicsView::handleTouch(QPointF position, int id)
{
    qDebug() << "Touch pressed at:" << position << "with ID:" << id;

    // ignore touches if not on a page
    if (current_page_id == 0)
    {
        return;
    }
    // only care about 1 touch
    currentTouchId = id;
    QPointF scenePos = this->mapToScene(position.toPoint());

    // Initialize current_path with a new QPainterPath
    auto current_path = QPainterPath(scenePos);

    // Create a Stroke using the current_path
    current_stroke = std::make_unique<Stroke>(current_path);
    current_stroke->room_id = state.room_id;
    current_stroke->page_id = current_page_id;
    current_stroke->owner_id = user_id;
    current_stroke->object_id = IDGenerator::newID();

    nlohmann::json event;
    current_stroke->createCreateEvent(event);

    std::string event_string = event.dump();
    ws_handler->handleEvent(event);
    ws_handler->sendEvent(event);
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
    ws_handler->sendEvent(event_json);
    ws_handler->handleEvent(event_json);
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

    ws_handler->sendEvent(event_json);
    ws_handler->handleEvent(event_json);

    current_stroke = nullptr;
    current_stroke_id = 0;
}

void TabletGraphicsView::resizeEvent(QResizeEvent *event)
{
    // Update the scene rectangle to match the new size of the view
    QGraphicsView::resizeEvent(event); // Call the base class implementation (optional if you don't want any default behavior)
    this->setSceneRect(0, 0, this->width() - 5, this->height() - 5);
}
