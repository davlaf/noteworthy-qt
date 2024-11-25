#include "newuser.h"
#include "AppNavigator.hpp"
#include "ui_newuser.h"
#include "widget.h"
#include <QMessageBox>  // Include for message box



NewUser::NewUser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewUser)
{
    ui->setupUi(this);
    ui->button->setCursor(Qt::PointingHandCursor);


}

NewUser::~NewUser()
{
    delete ui;
}


void NewUser::on_button_clicked()
{

    // check if the line edit is empty
    if (ui->lineEdit->text().isEmpty()) {
        // show a message box prompting the user to enter text
        ui->error->setText("Error: The input cannot be empty.");
        ui->error->setVisible(true);
    }else if (ui->lineEdit->text().length()> 100){
        ui->error->setText("Error: Input cannot exceed 100 characters.");
        ui->error->setVisible(true);
    }
    else {

        QString text = ui->lineEdit->text();
        // if the line edit has text, open the second window
        navigator->goToHomepage(text);
    }
}


void NewUser::on_lineEdit_returnPressed()
{
    // check if the line edit is empty
    if (ui->lineEdit->text().isEmpty()) {
        // show a message box prompting the user to enter text
        ui->error->setText("Error: The input cannot be empty.");
        ui->error->setVisible(true);
    }else if (ui->lineEdit->text().length()> 100){
        ui->error->setText("Error: Input cannot exceed 100 characters.");
        ui->error->setVisible(true);
    }
    else {

        QString text = ui->lineEdit->text();
        // if the line edit has text, open the second window
        navigator->goToHomepage(text);
    }
}


