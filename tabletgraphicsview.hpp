#pragma once

#include "ClientWebSocketHandler.hpp"
#include "RoomState.hpp"
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
    TabletGraphicsView(
        QWidget *parent = nullptr) : QGraphicsView(parent)
    {

        // uint64_t page_id,
        // std::shared_ptr<QGraphicsScene> scene,
        // std::shared_ptr<ClientWebSocketHandler> ws_handler,

        setAttribute(Qt::WA_AcceptTouchEvents); // Enable touch events
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // i dont know why I have to subtract 5
        this->setSceneRect(0, 0, this->width() - 5, this->height() - 5);

        this->pen = QColor{255, 50, 50};
        this->show();
    };

public:
    std::shared_ptr<ClientWebSocketHandler> ws_handler;

    void displayScene(std::shared_ptr<QGraphicsScene> scene)
    {

        this->setScene(scene.get());
    }

    uint64_t current_page_id;
    std::string user_id;

private:
    void resizeEvent(QResizeEvent *event) override;
    int currentTouchId;
    void handleTouch(QPointF position, int id);
    void handleRelease(QPointF position, int id);
    void handleMove(QPointF position, int id);
    QPen pen;

    std::unique_ptr<Stroke> current_stroke;
    uint64_t current_stroke_id;

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
