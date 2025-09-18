#include "ApiManager.h"
#include <QNetworkRequest>
#include <QJsonObject>
#include <QUrl>

ApiManager::ApiManager(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void ApiManager::postRequest(const QUrl &url) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // empty body, backend reads query parameter
    QNetworkReply *reply = manager->post(request, QByteArray());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            emit apiResponse(QJsonDocument::fromJson(response));
        } else {
            emit apiError(reply->errorString());
        }
        reply->deleteLater();
    });
}

void ApiManager::getRequest(const QUrl &url) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            emit apiResponse(QJsonDocument::fromJson(response));
        } else {
            emit apiError(reply->errorString());
        }
        reply->deleteLater();
    });
}
