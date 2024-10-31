#pragma once

#include "ui_RoomPage.h"
#include <QMainWindow>
#include <qgraphicsscene.h>
#include <QPainterPath>
#include <QEvent>
#include <QTouchEvent>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class RoomPage;
}
QT_END_NAMESPACE

class RoomPage : public QMainWindow
{
    Q_OBJECT

public:
    RoomPage(QWidget *parent = nullptr);
    ~RoomPage();
    std::string user_id;
    QGraphicsScene no_page_scene;

    void initialize();

private slots:
    void on_next_page_clicked();

    void on_prev_page_clicked();

private:
    Ui::RoomPage *ui;
};
