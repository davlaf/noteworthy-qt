#include "AppNavigator.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    navigator = new AppNavigator();
    navigator->goToWelcomePage();

    return a.exec();
}
