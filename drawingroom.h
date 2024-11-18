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

    void on_create_page_clicked();

    void on_delete_page_clicked();

    void on_previous_page_clicked();

    void on_next_page_clicked();

    void on_shape1_clicked();

    void on_shape2_clicked();

    void on_shape3_clicked();

    void on_shape4_clicked();

    void on_shape5_clicked();

    void on_shape6_clicked();

    void on_shape7_clicked();

    void on_shape8_clicked();

    void on_shape9_clicked();

    void on_symbol1_clicked();

    void on_symbol2_clicked();

    void on_symbol3_clicked();

    void on_symbol4_clicked();

    void on_symbol5_clicked();

    void on_symbol6_clicked();

    void on_symbol7_clicked();

    void on_symbol8_clicked();

    void on_symbol9_clicked();

    void on_copyCode_clicked();

private:
    Ui::drawingRoom *ui;
    int lastWidth = 0;
    QTimer *collapseCooldownTimer;
};

#endif // DRAWINGROOM_H
