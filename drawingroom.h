#ifndef DRAWINGROOM_H
#define DRAWINGROOM_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class drawingRoom;
}

class drawingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit drawingRoom(QWidget *parent = nullptr);
    ~drawingRoom();
    void setNameLabel(const QString &name);
    void setCodeLabel(const QString &codeText);
    void setButtonIconWithColor(QPushButton *button, const QString &svgPath, const QColor &color);
    void setUser(const QString &initial);


private slots:
    void on_collapse_clicked();

    void on_leaveRoom_clicked();

    void on_draw_clicked();

    void on_erase_clicked();

    void on_pages_clicked();

    void on_shapes_clicked();

    void on_symbols_clicked();

    void on_settings_clicked();

private:
    Ui::drawingRoom *ui;
    int lastWidth = 0;
    QTimer *collapseCooldownTimer;
};

#endif // DRAWINGROOM_H
