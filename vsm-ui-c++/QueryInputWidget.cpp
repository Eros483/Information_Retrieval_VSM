#include "QueryInputWidget.h"
#include "SearchBar.h"
#include <QVBoxLayout>
#include <QLabel>

QueryInputWidget::QueryInputWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *queryLabel = new QLabel("Enter Query", this);
    layout->addWidget(queryLabel);

    queryBar = new SearchBar(this);
    queryBar->setStyleSheet("background-color: rgba(0,0,0,0); color: white; border-radius: 10px;");
    layout->addWidget(queryBar);

    connect(queryBar, &SearchBar::searchRequested, this, [=](const QString &text) {
        emit queryEntered(text);
    });
}
