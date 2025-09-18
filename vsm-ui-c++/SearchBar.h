#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#endif // SEARCHBAR_H
#pragma once
#include <QLineEdit>

class SearchBar : public QLineEdit {
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);

signals:
    void searchRequested(const QString &text);
};
