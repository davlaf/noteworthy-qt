#pragma once

#include "RoomState.hpp"
#include "loadingroompage.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>
#include "newuser.h"
#include "widget.h"
#include "drawingroom.h"



class AppNavigator
{
public:
    // Homepage homepage;
    // WelcomePage welcome_page;
    // RoomPage room_page;
    NewUser welcome_page;
    Widget homepage;
    drawingRoom room_page;
    LoadingRoomPage loading_room_page;

    void goToWelcomePage() {
        welcome_page.show();
        homepage.hide();
        room_page.hide();
        loading_room_page.hide();
    }

    void goToHomepage(const QString& user_id) {
        homepage.setUsername(user_id);
        goToHomepage();
    }

    void goToHomepage() {
        homepage.show();
        welcome_page.hide();
        room_page.hide();
        loading_room_page.hide();
    }

    void goToRoomPageCreate(const std::string& room_id, const std::string& user_id) {
        // initialize room page
        state.room_id = room_id;
        state.owner_id = user_id;
        room_page.user_id = user_id;
        room_page.room_id = room_id;
        nlohmann::json json;
        state.createCreateRoomEvent(json);
        state.applyCreateRoomEvent(json);
        room_page.initialize();
        room_page.show();
        homepage.hide();
        welcome_page.hide();
        loading_room_page.hide();
    }

    enum RoomHTTPCodes {
        Ok = 200,
        Locked = 423,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404
    };

    void goToLoadingRoomPage(const std::string& room_id, const std::string& user_id) {

        loading_room_page.room_id = room_id;
        loading_room_page.setNameLabel(QString::fromStdString(user_id));
        loading_room_page.user_id = user_id;
        loading_room_page.show();
        homepage.hide();
        welcome_page.hide();
        room_page.hide();
    }

    void goToRoomPageJoin(const std::string& room_id, const std::string& user_id, const std::string password = "") {
        state.room_id = room_id;
        room_page.user_id = user_id;
        room_page.room_id = room_id;
        room_page.setCodeLabel(QString::fromStdString(room_id));
        room_page.setNameLabel(QString::fromStdString(user_id)+"'s Room");
        room_page.setUser(QString::fromStdString(user_id).at(0));

        fetchRoom(room_id, [room_id, user_id, this](std::string responseString, RoomHTTPCodes code){
            // epic
            switch (code) {
            case Ok:
                room_page.show();
                room_page.initialize(responseString);
                homepage.hide();
                welcome_page.hide();
                loading_room_page.hide();
                break;
            case Locked: {
                goToLoadingRoomPage(room_id, user_id);
                break;
            }
            case Unauthorized:
                throw "auth header is incorrect";
                break;
            case Forbidden:
                loading_room_page.setErrorText("Incorrect password");
                break;
            case NotFound:
                homepage.setErrorText("Invalid room code");
                break;
            }
        }, password);
    }

    QNetworkAccessManager manager;

    void fetchRoom(
        const std::string& room_id,
        const std::function<void(std::string, RoomHTTPCodes)> &onReply,
        const std::string password = "") {
        QUrl url(QString::fromStdString("http://localhost:8080/v1/rooms/"+room_id));
        // QUrl url(QString::fromStdString("https://noteworthy.howdoesthiseven.work/v1/rooms/"+room_id));
        QNetworkRequest request(url);

        // Set the authorization header
        if (password != "") {
            request.setRawHeader("Authorization", QByteArray("Bearer " + password));
        }

        // Send GET request
        QNetworkReply *reply = manager.get(request);

        QObject::connect(reply, &QNetworkReply::finished, [this, reply, onReply]() {
            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            if (!statusCode.isValid() || statusCode.toInt() >= 500) {
                homepage.setErrorText("Noteworthy server not online, please try again later");
                qDebug() << "HTTP Status Code not available";
                return;
            }

            QByteArray responseData = reply->readAll();
            std::string responseString = responseData.toStdString();

            RoomHTTPCodes httpStatusCode = static_cast<RoomHTTPCodes>(statusCode.toInt());

            onReply(responseString, httpStatusCode);
            reply->deleteLater(); // Clean up
        });
    }

    // void fetchRoomWithPassword
};

extern AppNavigator* navigator;
