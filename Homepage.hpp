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

private slots:
    void on_join_button_pressed();

private:
    Ui::Homepage *ui;
};
