#ifndef DRAWINGROOM_H
#define DRAWINGROOM_H

#include <QWidget>
#include <QPushButton>
#include <QGraphicsScene>

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
    std::string user_id;
    std::string room_id;
    QGraphicsScene no_page_scene;

    void initialize(std::string initial_room = "[]");

    void on_create_page_clicked();
    void on_delete_page_clicked();


    void handleCreatePageUIChange(uint64_t page_id);

    void handleDeletePageUIChange(uint64_t page_id);



    void on_output_room_clicked();



    void on_exit_clicked();



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
