#include "CorpusInputWidget.h"
#include "SearchBar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

CorpusInputWidget::CorpusInputWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    QLabel *corpusLabel = new QLabel("Enter Dir", this);
    hLayout->addWidget(corpusLabel);

    corpusBar = new SearchBar(this);
    corpusBar->setStyleSheet("background-color: rgba(0,0,0,0); color: white; border-radius: 10px;");
    hLayout->addWidget(corpusBar);

    browseButton = new QPushButton("Browse", this);
    hLayout->addWidget(browseButton);

    connect(browseButton, &QPushButton::clicked, this, [=]() {
        QString folder = QFileDialog::getExistingDirectory(this, "Select Corpus Directory");
        if (!folder.isEmpty()) {
            corpusBar->setText(folder);
            emit corpusSelected(folder);
        }
    });

    connect(corpusBar, &SearchBar::searchRequested, this, [=](const QString &text) {
        emit corpusSelected(text);
    });
}

QString CorpusInputWidget::corpusPath() const {
    return corpusBar->text();
}

void CorpusInputWidget::setBrowseVisible(bool visible) {
    if (browseButton) {
        browseButton->setVisible(visible);
    }
}
