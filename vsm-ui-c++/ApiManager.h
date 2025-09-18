#ifndef APIMANAGER_H
#define APIMANAGER_H

#endif // APIMANAGER_H
#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

class ApiManager : public QObject {
    Q_OBJECT
public:
    explicit ApiManager(QObject *parent = nullptr);

    void postRequest(const QUrl &url);
    void getRequest(const QUrl &url);

signals:
    void apiResponse(const QJsonDocument &response);
    void apiError(const QString &error);

private:
    QNetworkAccessManager *manager;
};
