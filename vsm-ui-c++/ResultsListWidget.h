#ifndef RESULTSLISTWIDGET_H
#define RESULTSLISTWIDGET_H

#endif // RESULTSLISTWIDGET_H
#pragma once
#include <QListWidget>
#include <QJsonArray>

class ResultsListWidget : public QListWidget {
    Q_OBJECT
public:
    explicit ResultsListWidget(QWidget *parent = nullptr);

    void showResults(const QJsonArray &results);
};
