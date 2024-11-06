#include "RoomPage.hpp"
#include "RandomIdGenerator.hpp"
#include "RoomState.hpp"
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

void RoomPage::initialize(std::string initial_room)
{
    ui->graphics->ws_handler = std::make_shared<ClientWebSocketHandler>();
    ui->graphics->ws_handler->openConnection();
    ui->graphics->setScene(&no_page_scene);
    ui->graphics->user_id = user_id;

    connect(
        ui->graphics->ws_handler.get(),
        &ClientWebSocketHandler::pageCreated,
        this,
        &RoomPage::handleCreatePageUIChange
        );

    connect(
        ui->graphics->ws_handler.get(),
        &ClientWebSocketHandler::pageDeleted,
        this,
        &RoomPage::handleDeletePageUIChange
        );

    nlohmann::json event_list = nlohmann::json::parse(initial_room);
    for (auto &event : event_list) {
        std::string epic = event.dump();
        ui->graphics->ws_handler->handleEvent(event);
    }

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

void RoomPage::on_create_page_clicked()
{
    nlohmann::json json;
    uint64_t new_id = IDGenerator::newID();
    state.createInsertPageEvent(json, ui->graphics->current_page_id, new_id);
    ui->graphics->ws_handler->sendEvent(json);
    state.applyInsertPageEvent(json);
    state.manipulatePage(new_id, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });
    ui->graphics->current_page_id = new_id;
    ui->page_label->setText("Page: " + QString::number(new_id));
}


void RoomPage::on_delete_page_clicked()
{
    // if after deleting page there are no pages left set scene to temporary scene
    uint64_t current_page_id = ui->graphics->current_page_id;

    nlohmann::json json;
    state.createDeletePageEvent(json, current_page_id);
    ui->graphics->ws_handler->sendEvent(json);
    handleDeletePageUIChange(current_page_id);
    state.applyDeletePageEvent(json);
}

void RoomPage::handleCreatePageUIChange(uint64_t page_id) {
    // nothing for now
};

void RoomPage::handleDeletePageUIChange(uint64_t page_id) {
    uint64_t current_page_id = ui->graphics->current_page_id;
    bool go_to_empty_page = false;
    uint64_t page_id_to_go_to;
    uint64_t prev_page_id;
    uint64_t next_page_id;
    // move user to previous page
    // if there is no prev page move to next page existing
    // otherwise show empty page
    if (state.getPrevPageId(current_page_id, prev_page_id)) {
        page_id_to_go_to = prev_page_id;
    } else if (state.getNextPageId(current_page_id, next_page_id)) {
        page_id_to_go_to = next_page_id;
    } else {
        go_to_empty_page = true;
    }

    if (go_to_empty_page) {
        ui->graphics->current_page_id = 0;
        ui->graphics->setScene(&no_page_scene);
        ui->page_label->setText("Page: ");
        return;
    }

    state.manipulatePage(page_id_to_go_to, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });

    ui->graphics->current_page_id = page_id_to_go_to;
    ui->page_label->setText("Page: " + QString::number(page_id_to_go_to));
};

void RoomPage::on_output_room_clicked()
{
    nlohmann::json json;

    state.toJsonEventList(json);

    std::string output = json.dump();
}

