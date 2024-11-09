#include "widget.h"
#include "ui_widget.h"
#include "drawingroom.h"
#include <QLabel>
#include <QRegularExpression>
#include <QRegularExpressionValidator> // For QRegularExpressionValidator
#include "newuser.h"




Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->lineEdit_5->setMaxLength(1);
    ui->lineEdit_6->setMaxLength(1);
    ui->lineEdit_7->setMaxLength(1);
    ui->lineEdit_8->setMaxLength(1);
    ui->lineEdit_9->setMaxLength(1);

    QRegularExpression regex("[A-Za-z0-9]");  // Allows only letters and numbers
    QValidator *validator = new QRegularExpressionValidator(regex, this);

    ui->lineEdit_5->setValidator(validator);
    ui->lineEdit_6->setValidator(validator);
    ui->lineEdit_7->setValidator(validator);
    ui->lineEdit_8->setValidator(validator);
    ui->lineEdit_9->setValidator(validator);

    ui->newpage->setCursor(Qt::PointingHandCursor);
    ui->codeEnter->setCursor(Qt::PointingHandCursor);
    ui->changename->setCursor(Qt::PointingHandCursor);
    ui->getfile->setCursor(Qt::PointingHandCursor);



}

Widget::~Widget()
{
    delete ui;
}

void Widget::setLabelText(const QString &text) {
    ui->name->setText(text);
}

void Widget::on_lineEdit_5_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->lineEdit_5->setFocus();  // Move back to previous line edit
        return;
    }

    if (arg1.length() == 1) {
        ui->lineEdit_5->setText(arg1.toUpper());
        ui->lineEdit_6->setFocus();
    }
}


void Widget::on_lineEdit_6_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->lineEdit_5->setFocus();  // Move back to previous line edit
        return;
    }

    if (arg1.length() == 1) {
        ui->lineEdit_6->setText(arg1.toUpper());
        ui->lineEdit_7->setFocus();
    }
}


void Widget::on_lineEdit_7_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->lineEdit_6->setFocus();  // Move back to previous line edit
        return;
    }

    if (arg1.length() == 1) {
        ui->lineEdit_7->setText(arg1.toUpper());
        ui->lineEdit_8->setFocus();
    }
}


void Widget::on_lineEdit_8_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->lineEdit_7->setFocus();  // Move back to previous line edit
        return;
    }

    if (arg1.length() == 1) {
        ui->lineEdit_8->setText(arg1.toUpper());
        ui->lineEdit_9->setFocus();
    }
}


void Widget::on_lineEdit_9_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->lineEdit_8->setFocus();  // Move back to previous line edit
        return;
    }

    if (arg1.length() == 1) {
        ui->lineEdit_9->setText(arg1.toUpper());
        ui->lineEdit_9->setFocus();
    }
}





void Widget::on_codeEnter_clicked()
{
    QString code = ui->lineEdit_5->text() +
                   ui->lineEdit_6->text() +
                   ui->lineEdit_7->text() +
                   ui->lineEdit_8->text() +
                   ui->lineEdit_9->text();
    QString name = ui->name->text();
    QString initial = ui->name->text().left(1);
    QString possessiveName = name + "'s Room";
    drawingRoom *joinroom = new drawingRoom();
    joinroom->setCodeLabel(code);  // Calls a method in drawingRoom to update the label
    joinroom->setNameLabel(possessiveName);  // Calls setNameLabel in Widget
    joinroom->setUser(initial);


    joinroom->show();


    // Close the main window
    this->close();
}

//go back to change name



void Widget::on_changename_clicked()
{
    NewUser *changename = new NewUser();
    changename->show();
    // Close the main window
    this->close();
}



void Widget::on_getfile_clicked()
{
    drawingRoom *newfile = new drawingRoom();
    newfile->show();

    // Close the main window
    this->close();
}



void Widget::on_newpage_clicked()
{
    drawingRoom *newpage = new drawingRoom();
    newpage->show();

    // Close the main window
    this->close();

}

