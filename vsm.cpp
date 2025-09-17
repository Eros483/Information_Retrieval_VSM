// vsm.cpp
#include "vsm.h"
#include "./ui_vsm.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

vsm::vsm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vsm)
{
    ui->setupUi(this);

    // Setup global hotkey
    toggleHotkey = new QHotkey(QKeySequence("Ctrl+Space"), true, this);
    if (toggleHotkey->isRegistered())
        qDebug() << "Hotkey registered successfully";

    connect(toggleHotkey, &QHotkey::activated, this, [=](){
        if (this->isVisible()) {
            this->hide();
        } else {
            this->show();
            this->raise();
            this->activateWindow();

            // Show only the active set
            if (queryBar->isVisible()) {
                queryBar->show();
                queryLabel->show();
                queryBar->setFocus();
            } else {
                corpusBar->show();
                corpusLabel->show();
                corpusBar->setFocus();
            }
        }
    });


    // Fullscreen transparent window
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    this->showFullScreen();
    this->hide(); // start hidden

    // Central widget
    QWidget *central = new QWidget(this);
    central->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setAlignment(Qt::AlignCenter);

    // GUI container
    QWidget *guiContainer = new QWidget(central);
    guiContainer->setFixedSize(600, 500);
    guiContainer->setStyleSheet("background-color: rgba(0,0,0,0); border-radius: 10px;");
    QVBoxLayout *layout = new QVBoxLayout(guiContainer);
    layout->setAlignment(Qt::AlignTop);

    QFont labelFont;
    labelFont.setPointSize(14);
    labelFont.setBold(true);

    // Corpus label
    corpusLabel = new QLabel("Enter Dir", this);
    corpusLabel->setFont(labelFont);
    corpusLabel->setStyleSheet("color: white;");
    layout->addWidget(corpusLabel);
    corpusLabel->hide();

    // Corpus bar
    corpusBar = new SearchBar(this);
    corpusBar->setStyleSheet("background-color: rgba(0,0,0,0); color: white; border-radius: 10px;");
    layout->addWidget(corpusBar);
    corpusBar->hide();

    // Query label
    queryLabel = new QLabel("Enter Query", this);
    queryLabel->setFont(labelFont);
    queryLabel->setStyleSheet("color: white;");
    layout->addWidget(queryLabel);
    queryLabel->hide();

    // Query bar
    queryBar = new SearchBar(this);
    queryBar->setStyleSheet("background-color: rgba(0,0,0,0); color: white; border-radius: 10px;");
    layout->addWidget(queryBar);
    queryBar->hide();

    // Error label
    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("QLabel { background-color: rgba(255,100,100,180); color: white; border-radius: 15px; padding: 8px; }");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->hide();
    layout->addWidget(errorLabel);

    // Results list
    resultsList = new QListWidget(this);
    resultsList->setStyleSheet(
        "QListWidget { background: transparent; border: none; color: white; }"
        "QListWidget::item { background-color: rgba(255,255,255,80); border: 1px solid rgba(255,255,255,100); border-radius: 12px; margin: 5px auto; padding: 10px; min-height: 50px; }"
        "QListWidget::item:selected { background-color: rgba(50,150,255,150); color: white; }"
        "QListWidget::item:hover { background-color: rgba(255,255,255,100); }"
        );
    resultsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    resultsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(resultsList);
    resultsList->hide();

    // Add container
    mainLayout->addWidget(guiContainer, 0, Qt::AlignCenter);

    // API manager
    api = new ApiManager(this);

    // Corpus search
    connect(corpusBar, &SearchBar::searchRequested, this, [=](const QString &text){
        errorLabel->hide();
        QUrl url("http://localhost:8000/build");
        QUrlQuery query;
        query.addQueryItem("corpus_dir", text);
        url.setQuery(query);
        api->postRequest(url);
    });

    // Query search
    connect(queryBar, &SearchBar::searchRequested, this, [=](const QString &text){
        errorLabel->hide();
        QUrl url("http://localhost:8000/search");
        QUrlQuery query;
        query.addQueryItem("query", text);
        url.setQuery(query);
        api->getRequest(url);
    });

    // API response
    connect(api, &ApiManager::apiResponse, this, [=](const QJsonDocument &doc){
        QJsonObject obj = doc.object();

        if (obj.contains("error")) {
            errorLabel->setText(obj["error"].toString());
            errorLabel->show();
            resultsList->hide();
            return;
        }

        if (corpusBar->isVisible()) {
            if (obj.contains("message") && obj["message"].toString().startsWith("Index built for corpus directory:")) {
                corpusBar->hide();
                corpusLabel->hide();

                queryBar->show();
                queryLabel->show();
                queryBar->setFocus();
            } else {
                errorLabel->setText(obj.contains("message") ? obj["message"].toString() : "Unknown error");
                errorLabel->show();
            }
        } else if (queryBar->isVisible()) {
            const QJsonArray results = obj["results"].toArray();
            resultsList->clear();

            if (results.isEmpty()) {
                QListWidgetItem *noResultsItem = new QListWidgetItem("No results found");
                noResultsItem->setTextAlignment(Qt::AlignCenter);
                resultsList->addItem(noResultsItem);
            } else {
                for (const QJsonValue &val : results) {
                    QJsonArray row = val.toArray();
                    if (row.size() < 2) continue;

                    QString filename = row[0].toString();
                    double score = row[1].toDouble();
                    QString scoreStr = QString::number(score * 100.0, 'f', 1) + "%";

                    QWidget *itemWidget = new QWidget();
                    QHBoxLayout *hlayout = new QHBoxLayout(itemWidget);
                    hlayout->setContentsMargins(15, 10, 15, 10);
                    hlayout->setSpacing(20);
                    hlayout->setAlignment(Qt::AlignCenter);

                    QLabel *iconLabel = new QLabel("📄");
                    iconLabel->setFixedSize(80, 80);
                    iconLabel->setAlignment(Qt::AlignCenter);

                    QLabel *fileLabel = new QLabel(filename);
                    QFont fileFont;
                    fileFont.setPointSize(16);
                    fileFont.setBold(true);
                    fileLabel->setFont(fileFont);
                    fileLabel->setWordWrap(true);

                    QLabel *scoreLabel = new QLabel(scoreStr);
                    QFont scoreFont;
                    scoreFont.setPointSize(14);
                    scoreFont.setBold(true);
                    scoreLabel->setFont(scoreFont);
                    scoreLabel->setAlignment(Qt::AlignCenter);

                    hlayout->addWidget(iconLabel);
                    hlayout->addWidget(fileLabel, 5);
                    hlayout->addWidget(scoreLabel);

                    QListWidgetItem *listItem = new QListWidgetItem();
                    listItem->setSizeHint(itemWidget->sizeHint());

                    resultsList->addItem(listItem);
                    resultsList->setItemWidget(listItem, itemWidget);
                }
            }

            resultsList->show();
        }
    });

    connect(api, &ApiManager::apiError, this, [=](const QString &err){
        errorLabel->setText(err);
        errorLabel->show();
        resultsList->hide();
    });
}

void vsm::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (queryBar->isVisible()) {
            queryBar->hide();
            queryLabel->hide();
            resultsList->hide();
            errorLabel->hide();

            corpusBar->show();
            corpusLabel->show();
            corpusBar->clear();
        }
    }
    QMainWindow::keyPressEvent(event);
}

vsm::~vsm() {
    delete ui;
}
