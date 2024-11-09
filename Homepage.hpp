#pragma once

#include "ui_Homepage.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Homepage;
}
QT_END_NAMESPACE


class Homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = nullptr);
    ~Homepage();
    QString username;

    void setUsername(const QString& username) {
        this->username = username;
        ui->name->setText(username);
    }

    void setErrorText(const QString& error) {
        ui->error->setText(error);
    }

private slots:
    void on_join_button_pressed();

    void on_create_room_clicked();

    void on_room_code_textChanged(const QString &arg1);

private:
    Ui::Homepage *ui;
};
