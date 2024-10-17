#include "tabletgraphicsview.h"

// void TabletGraphicsView::setNewStrokeCallback(void (*newStrokeCallback)(QPainterPath))
// {
//     this->newStrokeCallback = newStrokeCallback;
// }

TabletGraphicsView::TabletGraphicsView(QWidget *parent): QGraphicsView(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents);  // Enable touch events
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // i dont know why I have to subtract 5
    this->setSceneRect(0, 0, this->width()-5, this->height()-5);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);  // Align the scene at the top-left
    this->setResizeAnchor(QGraphicsView::AnchorViewCenter);  // Anchor resizing from the view's center
    qDebug() << "real shit";
    this->setScene(&scene);
    this->pen = QColor{255, 50, 50};
    QPainterPath epic {{0,0}};
    epic.lineTo(40,20);
    epic.lineTo(70,50);
    this->scene.addPath(epic, pen);
    this->show();
}

void TabletGraphicsView::handleTouch(QPointF position, int id)
{
    qDebug() << "Touch pressed at:" << position << "with ID:" << id;
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
    qDebug() << "Touch moved to:" << position << "with ID:" << id;
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

    qDebug() << "Touch released at:" << position << "with ID:" << id;
    // only care about 1 touch
    if (id != 0) {
        return;
    }
    // the callback takes ownership of the current path


    QPointF scenePos = this->mapToScene(position.toPoint());

    if (currentPath && currentPathItem) {
        // Finalize the path upon release
        currentPath->lineTo(scenePos);  // Ensure the path ends at the final touch point
        currentPathItem->setPath(*currentPath);  // Update the QGraphicsPathItem
        currentPath = nullptr;  // Reset the path for the next touch
        currentPathItem = nullptr;  // Reset the item for the next touch
    }
}

void TabletGraphicsView::resizeEvent(QResizeEvent *event) {
    // Update the scene rectangle to match the new size of the view
    QGraphicsView::resizeEvent(event); // Call the base class implementation (optional if you don't want any default behavior)
    this->setSceneRect(0, 0, this->width()-5, this->height()-5);
}
