#include "drawingroom.h"
#include "AppNavigator.hpp"
#include "RandomIdGenerator.hpp"
#include "ui_drawingroom.h"
#include "widget.h"
#include <QTabBar> // Add this line
#include <QPushButton>
#include <QIcon>
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTabWidget>
#include <QMessageBox>
#include <QHBoxLayout>  // For horizontal layout
#include <QTimer>
#include "widget.h"
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QTransform>
#include <QIcon>
#include <QPushButton>
#include "RoomState.hpp"






drawingRoom::drawingRoom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::drawingRoom)
{
    ui->setupUi(this);
    no_page_scene.addSimpleText(
        "Add a page to start!",
        QFont("Helvetica", 30));

    ui->graphics->setScene(&no_page_scene);

    ui->collapse->setCursor(Qt::PointingHandCursor);


    // Initialize the timer for the cooldown
    collapseCooldownTimer = new QTimer(this);
    collapseCooldownTimer->setSingleShot(true); // Only triggers once per cooldown

    // Re-enable the collapse button when the timer expires
    connect(collapseCooldownTimer, &QTimer::timeout, this, [this]() {
        ui->collapse->setEnabled(true);
    });



    ui->sidebartabs_2->tabBar()->setVisible(false);

    ui->sidebartabs_2->setContentsMargins(0, 0, 0, 0);


    //settings button
    ui->settings->setMaximumSize(32, 32); // Set size to be a square
    ui->settings->setStyleSheet("QPushButton {"
                                "border-radius: 16px;"
                                "border: none;"
                                "background-color: transparent;"
                                "}");

    QIcon settingsicon(":/svg/svg/settings.svg"); // Path to your SVG file in the resources or filesystem
    ui->settings->setIcon(settingsicon);
    ui->settings->setIconSize(QSize(20, 20)); // Set icon size if needed


    //symbols button
    ui->symbols->setMaximumSize(32, 32); // Set size to be a square
    ui->symbols->setStyleSheet("QPushButton {"
                               "border-radius: 16px;"
                               "border: none;"
                               "background-color: transparent;"
                               "}");

    QIcon icon(":/svg/svg/interests.svg"); // Path to your SVG file in the resources or filesystem
    ui->symbols->setIcon(icon);
    ui->symbols->setIconSize(QSize(20, 20)); // Set icon size if needed


    //shapes button
    ui->shapes->setMaximumSize(32, 32); // Set size to be a square
    ui->shapes->setStyleSheet("QPushButton {"
                              "border-radius: 16px;"
                              "border: none;"
                              "background-color: transparent;"
                              "}");

    QIcon icon1(":/svg/svg/shapes.svg"); // Path to your SVG file in the resources or filesystem
    ui->shapes->setIcon(icon1);
    ui->shapes->setIconSize(QSize(20, 20)); // Set icon size if needed

    //pages button
    ui->pages->setMaximumSize(32, 32); // Set size to be a square
    ui->pages->setStyleSheet("QPushButton {"
                             "border-radius: 20px;"
                             "background-color: rgba(174, 243, 163, 1);"
                             "}");
    QIcon icon2(":/svg/svg/select_window_2.svg"); // Path to your SVG file in the resources or filesystem
    ui->pages->setIcon(icon2);
    ui->pages->setIconSize(QSize(20, 20)); // Set icon size if needed

    //collapse button
    ui->collapse->setMaximumSize(40, 40); // Set size to be a square
    ui->collapse->setStyleSheet("QPushButton {"

                                "}");

    QIcon icon3(":/svg/svg/menu_24dp_E8EAED_FILL0_wght400_GRAD0_opsz24.svg"); // Path to your SVG file in the resources or filesystem
    ui->collapse->setIcon(icon3);
    ui->collapse->setIconSize(QSize(30, 30)); // Set icon size if needed
    setButtonIconWithColor(ui->collapse, ":/svg/svg/menu_24dp_E8EAED_FILL0_wght400_GRAD0_opsz24.svg", QColor(74, 160, 60));

    //leave room button
    ui->leaveRoom->setMaximumSize(40, 40); // Set size to be a square
    ui->leaveRoom->setStyleSheet("QPushButton {"

                             "}");

    QIcon icon5(":/svg/svg/arrow_back_ios_new.svg"); // Path to your SVG file in the resources or filesystem
    ui->leaveRoom->setIcon(icon5);
    ui->leaveRoom->setIconSize(QSize(19, 19)); // Set icon size if needed

    //select button
    ui->select->setMaximumSize(40, 40); // Set size to be a square
    ui->select->setStyleSheet("QPushButton {"
                             "background-color: rgba(174, 243, 163, 1);"
                              "padding-left: 10px; padding-right: 10px;"
                             "border-radius: 20px;"   // Radius = half of the width/height
                             "}");
    QIcon icon6(":/svg/svg/select.svg"); // Path to your SVG file in the resources or filesystem
    ui->select->setIcon(icon6);
    ui->select->setIconSize(QSize(23, 23)); // Set icon size if needed


    //save button
    ui->save->setMaximumSize(40, 40); // Set size to be a square
    ui->save->setStyleSheet("QPushButton {"
                              "background-color: rgba(174, 243, 163, 1);"
                              "border-radius: 20px;"   // Radius = half of the width/height
                            "padding-left: 10px; padding-right: 10px;"
                              "}");
    QIcon icon7(":/svg/svg/save.svg"); // Path to your SVG file in the resources or filesystem
    ui->save->setIcon(icon7);
    ui->save->setIconSize(QSize(23, 23)); // Set icon size if needed


    //article button
    ui->article->setMaximumSize(40, 40); // Set size to be a square
    ui->article->setStyleSheet("QPushButton {"
                            "background-color: rgba(174, 243, 163, 1);"
                            "border-radius: 20px;"   // Radius = half of the width/height
                            "padding-left: 10px; padding-right: 10px;"
                            "}");
    QIcon icon8(":/svg/svg/article.svg"); // Path to your SVG file in the resources or filesystem
    ui->article->setIcon(icon8);
    ui->article->setIconSize(QSize(23, 23)); // Set icon size if needed


    //copy code button
    ui->copyCode->setMaximumSize(40, 40); // Set size to be a square
    ui->copyCode->setStyleSheet("QPushButton {"
                               "background-color: rgba(174, 243, 163, 1);"
                               "border-radius: 20px;"
                                "padding-left: 10px; padding-right: 10px;"
                               "}");
    QIcon icon9(":/svg/svg/content_copy.svg"); // Path to your SVG file in the resources or filesystem
    ui->copyCode->setIcon(icon9);
    ui->copyCode->setIconSize(QSize(23, 23)); // Set icon size if needed

    //draw button
    ui->draw->setMaximumSize(32, 32); // Set size to be a square
    ui->draw->setStyleSheet("QPushButton {"
                                "border-radius: 16px;"
                                "background-color: rgba(174, 243, 163, 1);"
                                "}");
    QIcon icon10(":/svg/svg/draw.svg"); // Path to your SVG file in the resources or filesystem
    ui->draw->setIcon(icon10);
    ui->draw->setIconSize(QSize(20, 20)); // Set icon size if needed

    //erase button
    ui->erase->setMaximumSize(32, 32); // Set size to be a square
    ui->erase->setStyleSheet("QPushButton {"
                                "border-radius: 16px;"
                                "border: none;"
                                "background-color: transparent;"
                                "}");
    QIcon icon11(":/svg/svg/ink_eraser.svg"); // Path to your SVG file in the resources or filesystem
    ui->erase->setIcon(icon11);
    ui->erase->setIconSize(QSize(20, 20)); // Set icon size if needed




}
void drawingRoom::setNameLabel(const QString &name) {
    ui->name->setText(name);  // Set the label in Widget
}





drawingRoom::~drawingRoom()
{
    delete ui;
}

void drawingRoom::on_collapse_clicked()
{


    if (!ui->collapse->isEnabled()) return;  // Return if button is disabled

    ui->collapse->setEnabled(false);  // Disable button for cooldown
    collapseCooldownTimer->start(300);  // Start the 300ms cooldown timer


    QPropertyAnimation *animationMin = new QPropertyAnimation(ui->sidebar, "minimumWidth");
    QPropertyAnimation *animationMax = new QPropertyAnimation(ui->sidebar, "maximumWidth");

    animationMin->setDuration(300);
    animationMax->setDuration(300);

    static QPoint originalPos;  // Store the original position for reparenting

    if (ui->sidebar->width() > 80) { // If sidebar is currently expanded
        lastWidth = ui->sidebar->width();
        animationMin->setStartValue(lastWidth);
        animationMin->setEndValue(80);  // Collapse to 60 width
        animationMax->setStartValue(lastWidth);
        animationMax->setEndValue(80);


        originalPos = ui->collapse->mapToGlobal(QPoint(0, 0));





        animationMin->start(QAbstractAnimation::DeleteWhenStopped);
        animationMax->start(QAbstractAnimation::DeleteWhenStopped);
        ui->collapse->setStyleSheet(
            "background-color: transparent"


            );


        QTimer::singleShot(300, this, [this]() {
            ui->collapse->setParent( this);  // Reparent to the main window or a different parent
            ui->collapse->move(mapFromGlobal(originalPos));  // Move to original global position

            ui->collapse->show();  // Ensure it's visible
            // Hide all child widgets except the collapse button
            for (QObject *child : ui->sidebar->children()) {
                if (QWidget *widget = qobject_cast<QWidget *>(child)) {
                    if (child != ui->collapse) {
                        widget->setVisible(false); // Hide child widgets
                    }
                }
            }
        });


    } else { // If sidebar is currently collapsed
        // Set the collapse button's parent
        ui->collapse->setParent(ui->widget_8);

        // Get the horizontal layout from widget_8
        QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(ui->widget_8->layout());
        if (layout) {
            // Insert the collapse button at the front of the layout
            layout->insertWidget(0, ui->collapse); // Insert at index 0 to place it at the front
        }

        ui->collapse->show(); // Show the button



        animationMin->setStartValue(80);
        animationMin->setEndValue(lastWidth);  // Expand to full width
        animationMax->setStartValue(80);
        animationMax->setEndValue(lastWidth);




        for (QObject *child : ui->sidebar->children()) {
            if (QWidget *widget = qobject_cast<QWidget *>(child)) {
                widget->setVisible(true); // Show children
            }
        }

        animationMin->start(QAbstractAnimation::DeleteWhenStopped);
        animationMax->start(QAbstractAnimation::DeleteWhenStopped);
    }








}


void drawingRoom::setCodeLabel(const QString &codeText) {
    ui->code->setText(codeText);  // `codeLabel` is the QLabel in `drawingRoom`
}



void drawingRoom::setUser(const QString &initial) {
    ui->user1->setText(initial);  // `codeLabel` is the QLabel in `drawingRoom`
}

void drawingRoom::setButtonIconWithColor(QPushButton *button, const QString &svgPath, const QColor &color) {
    // Load the SVG renderer
    QSvgRenderer renderer(svgPath);

    // Set desired dimensions for the icon
    int iconDimension = 30;
    QSize iconSize(iconDimension, 40);

    // Create a square pixmap for rendering
    QPixmap pixmap(iconSize);
    pixmap.fill(Qt::transparent);

    // Create a QPainter to render the SVG onto the pixmap
    QPainter painter(&pixmap);
    renderer.render(&painter);
    painter.end();

    // Set color overlay with CompositionMode_SourceIn to colorize the icon
    QPainter colorPainter(&pixmap);
    colorPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    colorPainter.fillRect(pixmap.rect(), color); // Apply the specified color
    colorPainter.end();

    // Set the final colorized pixmap as the icon
    button->setIcon(QIcon(pixmap));
    button->setIconSize(iconSize); // Ensure icon fits the button size
}


void drawingRoom::on_leaveRoom_clicked()
{
    navigator->goToHomepage(QString::fromStdString(user_id));
}


void drawingRoom::on_draw_clicked()
{
    ui->draw->setStyleSheet("QPushButton {"
                            "background-color: rgba(174, 243, 163, 1);"
                            "border-radius: 16px;"
                            "}");

    ui->erase->setStyleSheet("QPushButton {"
                             "background-color: transparent;"
                             "border-radius: 16px;"
                             "border: none;"
                             "}");
}


void drawingRoom::on_erase_clicked()
{
    ui->erase->setStyleSheet("QPushButton {"
                            "background-color: rgba(174, 243, 163, 1);"
                            "border-radius: 16px;"
                            "}");

    ui->draw->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");
}




void drawingRoom::on_pages_clicked()
{
    ui->sidebartabs_2->setCurrentIndex(0);
    ui->pages->setStyleSheet("QPushButton {"
                             "background-color: rgba(174, 243, 163, 1);"
                             "border-radius: 20px;"
                             "}");

    ui->shapes->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->symbols->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->settings->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");
}


void drawingRoom::on_shapes_clicked()
{
    ui->sidebartabs_2->setCurrentIndex(1);

    ui->shapes->setStyleSheet("QPushButton {"
                             "background-color: rgba(174, 243, 163, 1);"
                             "border-radius: 20px;"
                             "}");

    ui->pages->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->symbols->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->settings->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");
}


void drawingRoom::on_symbols_clicked()
{
    ui->sidebartabs_2->setCurrentIndex(2);

    ui->symbols->setStyleSheet("QPushButton {"
                             "background-color: rgba(174, 243, 163, 1);"
                             "border-radius: 20px;"
                             "}");

    ui->pages->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->shapes->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->settings->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");
}

void drawingRoom::on_settings_clicked()
{
    ui->sidebartabs_2->setCurrentIndex(3);

    ui->settings->setStyleSheet("QPushButton {"
                             "background-color: rgba(174, 243, 163, 1);"
                             "border-radius: 20px;"
                             "}");

    ui->pages->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->shapes->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");

    ui->symbols->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 16px;"
                            "border: none;"
                            "}");
}


void drawingRoom::initialize(std::string initial_room)
{
    ui->graphics->ws_handler = std::make_shared<ClientWebSocketHandler>();
    ui->graphics->ws_handler->openConnection();
    ui->graphics->setScene(&no_page_scene);
    ui->graphics->user_id = user_id;

    ui->code->setText(QString::fromStdString(room_id));

    connect(
        ui->graphics->ws_handler.get(),
        &ClientWebSocketHandler::pageCreated,
        this,
        &drawingRoom::handleCreatePageUIChange
        );

    connect(
        ui->graphics->ws_handler.get(),
        &ClientWebSocketHandler::pageDeleted,
        this,
        &drawingRoom::handleDeletePageUIChange
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
    //ui->page_label->setText("Page: " + QString::number(first_page_id));
}


void drawingRoom::handleCreatePageUIChange(uint64_t page_id) {
    // nothing for now

    // get the page
    state.manipulatePage(page_id, [this](Page &page){
        // scroll view in the slides tab
        // create a widget with rounded corners and a border and inside is a QGraphicsView
        ClickableGraphicsView* tempThumbnail = new ClickableGraphicsView;
        page.thumbnail = tempThumbnail;
        page.thumbnail->setMinimumSize(0, 0);
        page.thumbnail->setMaximumSize(16777215, 16777215);
        page.thumbnail->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        page.thumbnail->setStyleSheet("background-color: white; "
                                       "border: 3px solid black; "
                                       "border-radius: 15px");
        page.thumbnail->resize(ui->widget_3->width(), (637/951)*ui->widget_3->width());

        ThumbnailList.append(page.thumbnail); // add thumbnail to list of thumbnails

        //int insertPosition = ui->widget_3->layout()->indexOf(ui->create_page) - 1;
        //ui->widget_3->layout()->insertWidget(rectangleWidget);
        ui->widget_3->layout()->addWidget(page.thumbnail);
        //connect(page.thumbnail, &SenderClass::signalName, receiverObject, &ReceiverClass::slotName);

    });
};

void drawingRoom::handleDeletePageUIChange(uint64_t page_id) {
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
        return;
    }

    state.manipulatePage(page_id_to_go_to, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });

    ui->graphics->current_page_id = page_id_to_go_to;
};

// void drawingRoom::on_output_room_clicked()
// {
//     nlohmann::json json;

//     state.toJsonEventList(json);

//     std::string output = json.dump();
// }


// void drawingRoom::on_exit_clicked()
// {
//     navigator->goToHomepage();
// }


void drawingRoom::on_create_page_clicked()
{
    nlohmann::json json;
    uint64_t new_id = IDGenerator::newID();
    state.createInsertPageEvent(json, ui->graphics->current_page_id, new_id);
    ui->graphics->ws_handler->sendEvent(json);
    ui->graphics->ws_handler->handleEvent(json);
    state.manipulatePage(new_id, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });
    ui->graphics->current_page_id = new_id;
}


void drawingRoom::on_delete_page_clicked()
{
    // if after deleting page there are no pages left set scene to temporary scene
    uint64_t current_page_id = ui->graphics->current_page_id;

    nlohmann::json json;
    state.createDeletePageEvent(json, current_page_id);
    ui->graphics->ws_handler->sendEvent(json);
    ui->graphics->ws_handler->handleEvent(json);
}

void drawingRoom::on_previous_page_clicked()
{
    uint64_t prev_page_id;
    if (!state.getPrevPageId(ui->graphics->current_page_id, prev_page_id))
    {
        return;
    }
    state.manipulatePage(prev_page_id, [this](Page &page)
                         { ui->graphics->displayScene(page.scene); });
    ui->graphics->current_page_id = prev_page_id;
}


void drawingRoom::on_next_page_clicked()
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
}

