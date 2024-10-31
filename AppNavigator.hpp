#pragma once

#include "Homepage.hpp"
#include "RoomPage.hpp"
#include "WelcomePage.hpp"
class AppNavigator
{
public:
    Homepage homepage;
    WelcomePage welcome_page;
    RoomPage room_page;

    void goToWelcomePage() {
        welcome_page.show();
        homepage.hide();
        room_page.hide();
    }

    void goToHomepage(const QString& user_id) {
        homepage.setUsername(user_id);
        goToHomepage();
    }

    void goToHomepage() {
        homepage.show();
        welcome_page.hide();
        room_page.hide();
    }

    void goToRoomPage(const std::string& room_id, const std::string& user_id) {
        // initialize room page
        state.room_id = room_id;
        room_page.user_id = user_id;
        room_page.initialize();
        room_page.show();
        homepage.hide();
        welcome_page.hide();
    }
};

extern AppNavigator* navigator;
