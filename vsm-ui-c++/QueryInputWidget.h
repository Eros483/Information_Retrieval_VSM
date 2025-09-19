#ifndef QUERYINPUTWIDGET_H
#define QUERYINPUTWIDGET_H

#include <QWidget>

class SearchBar;

class QueryInputWidget : public QWidget {
    Q_OBJECT
public:
    explicit QueryInputWidget(QWidget *parent = nullptr);

signals:
    void queryEntered(const QString &query);

private:
    SearchBar *queryBar;
};

#endif // QUERYINPUTWIDGET_H
