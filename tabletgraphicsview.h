#ifndef TABLETGRAPHICSVIEW_H
#define TABLETGRAPHICSVIEW_H

#include "ClientWebSocketHandler.hpp"
#include "Stroke.hpp"
#include <QGraphicsView>
#include <QWidget>
#include <qgraphicsscene.h>
#include <QPainterPath>
#include <QEvent>
#include <QTouchEvent>
#include <QGraphicsPathItem>
#include <qwebsocket.h>

class TabletGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    TabletGraphicsView(QWidget *parent = nullptr);

private:
    void resizeEvent(QResizeEvent *event) override;
    int currentTouchId;
    void handleTouch(QPointF position, int id);
    void handleRelease(QPointF position, int id);
    void handleMove(QPointF position, int id);

    enum CanvasObject::ObjectType current_object = CanvasObject::STROKE;
    enum CanvasObject::EventType current_transform = CanvasObject::CREATE;

    std::shared_ptr<QGraphicsScene> scene = std::make_shared<QGraphicsScene>();
    QPen pen;

    ClientWebSocketHandler web_socket_handler;
    std::unique_ptr<Stroke> current_stroke;
    uint64_t current_stroke_id;

    // TODO: make it not hard coded
    uint64_t selected_page_id = 12345;

protected:
    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::TouchBegin ||
            event->type() == QEvent::TouchUpdate ||
            event->type() == QEvent::TouchEnd)
        {

            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);

            for (const QTouchEvent::TouchPoint &point : touchEvent->points())
            {
                int id = point.id();            // ID of the touch point
                QPointF pos = point.position(); // Current position of the touch point
                Qt::TouchPointState pointState = static_cast<Qt::TouchPointState>(point.state());
                switch (pointState)
                {
                case Qt::TouchPointPressed:
                    handleTouch(pos, id);
                    break;
                case Qt::TouchPointMoved:
                    handleMove(pos, id);
                    break;
                case Qt::TouchPointReleased:
                    handleRelease(pos, id);
                    break;
                default:
                    break;
                }
            }
            return true; // Event has been handled
        }
        return QGraphicsView::event(event); // Pass the event to the base class if not handled
    }
};

#endif // TABLETGRAPHICSVIEW_H
