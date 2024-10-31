#include "RoomPage.hpp"
#include "./ui_RoomPage.h"

RoomPage::RoomPage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RoomPage)
{
    ui->setupUi(this);
    no_page_scene.addSimpleText(
        "Add a page to start!",
        QFont("Helvetica", 30));

    ui->graphics->setScene(&no_page_scene);
}

RoomPage::~RoomPage()
{
    delete ui;
}

void RoomPage::initialize()
{
    ui->graphics->ws_handler = std::make_shared<ClientWebSocketHandler>();
    ui->graphics->ws_handler->openConnection();
    ui->graphics->setScene(&no_page_scene);
    ui->graphics->user_id = user_id;

    uint64_t first_page_id;
    if (!state.getFirstPageId(first_page_id))
    {
        return;
    }

    ui->graphics->current_page_id = first_page_id;
    state.manipulatePage(first_page_id, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });
    ui->page_label->setText("Page: " + QString::number(first_page_id));
}

void RoomPage::on_next_page_clicked()
{
    // find the current page in the list
    uint64_t next_page_id;
    if (!state.getNextPageId(ui->graphics->current_page_id, next_page_id))
    {
        return;
    }
    state.manipulatePage(next_page_id, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });
    ui->graphics->current_page_id = next_page_id;
    ui->page_label->setText("Page: " + QString::number(next_page_id));
}

void RoomPage::on_prev_page_clicked()
{
    uint64_t prev_page_id;
    if (!state.getPrevPageId(ui->graphics->current_page_id, prev_page_id))
    {
        return;
    }
    state.manipulatePage(prev_page_id, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });
    ui->graphics->current_page_id = prev_page_id;
    ui->page_label->setText("Page: " + QString::number(prev_page_id));
}
