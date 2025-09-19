#include "ResultsListWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QListWidgetItem>

ResultsListWidget::ResultsListWidget(QWidget *parent) : QListWidget(parent) {
    setStyleSheet(
        "QListWidget { background: transparent; border: none; color: white; }"
        "QListWidget::item { border: none; margin: 8px auto; min-height: 50px; }"
        "QListWidget::item:selected { background: rgba(50,150,255,150); color: white; border-radius: 12px; }"
        "QListWidget::item:hover { background: rgba(255,255,255,40); border-radius: 12px; }"
        );
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ResultsListWidget::showResults(const QJsonArray &results) {
    clear();

    if (results.isEmpty()) {
        QListWidgetItem *noResultsItem = new QListWidgetItem("No results found");
        noResultsItem->setTextAlignment(Qt::AlignCenter);
        addItem(noResultsItem);
        return;
    }

    for (const QJsonValue &val : results) {
        QJsonArray row = val.toArray();
        if (row.size() < 2) continue;

        QString filename = row[0].toString();

        QWidget *itemWidget = new QWidget();
        QHBoxLayout *hlayout = new QHBoxLayout(itemWidget);
        hlayout->setContentsMargins(10, 6, 10, 6);
        hlayout->setSpacing(12);

        // File icon
        QLabel *iconLabel = new QLabel("📄");
        iconLabel->setFixedSize(28, 28);
        iconLabel->setAlignment(Qt::AlignCenter);
        hlayout->addWidget(iconLabel);

        // File name
        QLabel *fileLabel = new QLabel(filename);
        QFont fileFont;
        fileFont.setPointSize(14);
        fileFont.setBold(true);
        fileLabel->setFont(fileFont);
        fileLabel->setStyleSheet("color: white;");
        fileLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        fileLabel->setWordWrap(true);
        hlayout->addWidget(fileLabel);

        itemWidget->setLayout(hlayout);

        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(QSize(0, 50));
        addItem(listItem);
        setItemWidget(listItem, itemWidget);
    }
}
