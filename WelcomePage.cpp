#include "WelcomePage.hpp"
#include "AppNavigator.hpp"
#include "Homepage.hpp"
#include "ui_WelcomePage.h"

WelcomePage::WelcomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WelcomePage)
{
    ui->setupUi(this);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}

void WelcomePage::on_next_pressed()
{
    navigator->goToHomepage(ui->name->text());
}

