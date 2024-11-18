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

    // Check if the line edit is empty
    if (ui->lineEdit->text().isEmpty()) {
        // Show a message box prompting the user to enter text

    } else {

        QString text = ui->lineEdit->text();

        // If the line edit has text, open the second window
        navigator->goToHomepage(text);
    }
}

