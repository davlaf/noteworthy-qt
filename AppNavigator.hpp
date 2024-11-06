#pragma once

#include "Homepage.hpp"
#include "RoomPage.hpp"
#include "WelcomePage.hpp"
#include "RoomState.hpp"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>

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

    void goToRoomPageCreate(const std::string& room_id, const std::string& user_id) {
        // initialize room page
        state.room_id = room_id;
        state.owner_id = user_id;
        room_page.user_id = user_id;
        nlohmann::json json;
        state.createCreateRoomEvent(json);
        state.applyCreateRoomEvent(json);
        room_page.initialize();
        room_page.show();
        homepage.hide();
        welcome_page.hide();
    }

    enum RoomHTTPCodes {
        Ok = 200,
        Locked = 423,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404
    };

    std::string getInputFromDialog(QWidget *parent = nullptr) {
        QDialog dialog(parent);
        dialog.setWindowTitle("Input Dialog");

        // Create a label, input field, and buttons
        QLabel label("Enter your password:", &dialog);
        QLineEdit passwordField(&dialog);
        passwordField.setEchoMode(QLineEdit::Password);  // Set to password mode
        QPushButton okButton("OK", &dialog);
        QPushButton cancelButton("Cancel", &dialog);

        // Connect the OK and Cancel buttons
        QObject::connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        QObject::connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

        // Layout for dialog
        QVBoxLayout layout;
        layout.addWidget(&label);
        layout.addWidget(&passwordField);
        layout.addWidget(&okButton);
        layout.addWidget(&cancelButton);
        dialog.setLayout(&layout);

        // Execute the dialog
        if (dialog.exec() == QDialog::Accepted) {
            return passwordField.text().toStdString();
        } else {
            return "";  // Return an empty string if the dialog was canceled
        }
    }

    void goToRoomPageJoin(const std::string& room_id, const std::string& user_id) {
        state.room_id = room_id;
        room_page.user_id = user_id;

        fetchRoom(room_id, [room_id, this](std::string string, RoomHTTPCodes code){
            // epic
            switch (code) {
            case Ok:
                room_page.show();
                room_page.initialize(string);
                homepage.hide();
                welcome_page.hide();
                break;
            case Locked: {
                std::string password = getInputFromDialog();
                fetchRoom(room_id, [this](std::string string, RoomHTTPCodes code){
                    switch (code) {
                    case Ok:
                        room_page.show();
                        room_page.initialize(string);
                        homepage.hide();
                        welcome_page.hide();
                        break;
                    case Locked:
                        throw "no password?";
                        break;
                    case Unauthorized:
                        throw "auth header is incorrect";
                        break;
                    case Forbidden:
                        // wrong password
                        break;
                    case NotFound:
                        break;
                    }
                }, password);
                break;
            }
            case Unauthorized:
                throw "auth header is incorrect";
                break;
            case Forbidden:
                throw "there shouldn't have been a password";
                break;
            case NotFound:
                QMessageBox::information(nullptr, "Error", "No room found with that code");
                break;
            }
        });

    }

    QNetworkAccessManager manager;

    // const std::function<void(RoomState &)> &manipulator =

    void fetchRoom(
        const std::string& room_id,
        const std::function<void(std::string, RoomHTTPCodes)> &onReply,
        const std::string password = "") {
        QUrl url(QString::fromStdString("http://localhost:8080/rooms/"+room_id));
        QNetworkRequest request(url);

        // Set the authorization header
        if (password != "") {
            request.setRawHeader("Authorization", QByteArray("Bearer " + password));
        }

        // Send GET request
        QNetworkReply *reply = manager.get(request);

        QObject::connect(reply, &QNetworkReply::finished, [reply, onReply]() {

            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            if (!statusCode.isValid()) {
                QMessageBox::information(nullptr, "Error", "Noteworthy server not online, please try again later");
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
