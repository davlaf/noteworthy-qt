#include "Homepage.hpp"
#include "AppNavigator.hpp"
#include "ui_Homepage.h"

Homepage::Homepage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Homepage)
{
    ui->setupUi(this);
}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::on_join_button_pressed()
{
    navigator->goToRoomPageJoin(ui->room_code->text().toStdString(), username.toStdString());
}

