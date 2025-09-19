#ifndef ERRORBANNER_H
#define ERRORBANNER_H

#endif // ERRORBANNER_H
#pragma once
#include <QLabel>

class ErrorBanner : public QLabel {
    Q_OBJECT
public:
    explicit ErrorBanner(QWidget *parent = nullptr);
    void showError(const QString &msg);
};
