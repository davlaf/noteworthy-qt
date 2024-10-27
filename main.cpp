#include "RoomState.hpp"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    auto new_page = std::make_unique<Page>();
    state.addPageAfter(0, 12345, std::move(new_page));
    return a.exec();
}
