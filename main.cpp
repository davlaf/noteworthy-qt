#include "AppNavigator.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    navigator = new AppNavigator();
    navigator->goToWelcomePage();

    std::unique_ptr<Page> page = std::make_unique<Page>();

    page->page_id = 12345;
    state.addPageAfter(0, std::move(page));

    std::unique_ptr<Page> page2 = std::make_unique<Page>();

    page2->page_id = 23456;
    state.addPageAfter(12345, std::move(page2));

    return a.exec();
}
