#include "newuser.h"
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
        QMessageBox msgBox;
        msgBox.setStyleSheet("QLabel { color: white; }");  // Sets the text color in QMessageBox to black
        msgBox.setText("Please Enter a Name");
        msgBox.exec();


    } else {

        QString text = ui->lineEdit->text();

        // If the line edit has text, open the second window
        Widget *roomwait = new Widget();
        roomwait->setLabelText(text);  // Set the text on the second window's label
        roomwait->show();

        // Close the main window
        this->close();
    }
}

