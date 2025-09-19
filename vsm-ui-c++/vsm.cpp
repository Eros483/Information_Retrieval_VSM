#include "vsm.h"
#include "./ui_vsm.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressBar>

vsm::vsm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vsm)
{
    ui->setupUi(this);

    // ----------------- Select API   -----------------
    QComboBox *apiSelector = new QComboBox(this);
    apiSelector->addItem("localhost");
    apiSelector->addItem("hosted");
    apiSelector->setCurrentIndex(0);
    //localhost by default

    // ----------------- Window Style -----------------
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    showFullScreen();
    hide(); // start hidden

    // ----------------- Central Layout -----------------
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setAlignment(Qt::AlignCenter);

    mainLayout->insertWidget(0, apiSelector, 0, Qt::AlignTop);

    // ----------------- Progress bar -----------------
    loadingBar = new QProgressBar(this);
    loadingBar->setRange(0, 0);
    loadingBar->setFixedSize(200, 4);
    loadingBar->setTextVisible(false);
    loadingBar->hide();

    QWidget *loadingContainer = new QWidget(this);
    QHBoxLayout *loadingLayout = new QHBoxLayout(loadingContainer);
    loadingLayout->addWidget(loadingBar, 0, Qt::AlignCenter);
    loadingLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->insertWidget(1, loadingContainer, 0, Qt::AlignCenter);

    // ----------------- API change -----------------

    connect(apiSelector, &QComboBox::currentTextChanged, this, [=](const QString &choice) {
        corpusWidget->show();
        queryWidget->hide();
        resultsWidget->hide();
        errorBanner->hide();
        loadingBar->hide();

        if (choice == "localhost") {
            baseUrl = "http://localhost:8000";
            corpusWidget->setBrowseVisible(true);   // show browse button
        } else {
            baseUrl = "https://information-retrieval-vsm.onrender.com";
            corpusWidget->setBrowseVisible(false);  // hide browse button

            // call hosted root API
            QUrl url(baseUrl + "/");
            api->getRequest(url);
        }
    });




    // ----------------- GUI Container -----------------
    QWidget *guiContainer = new QWidget(central);
    guiContainer->setFixedSize(600, 500);
    guiContainer->setStyleSheet("background-color: rgba(0,0,0,0); border-radius: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(guiContainer);
    layout->setAlignment(Qt::AlignTop);


    // ----------------- Widgets -----------------
    corpusWidget = new CorpusInputWidget(this);
    queryWidget = new QueryInputWidget(this);
    resultsWidget = new ResultsListWidget(this);
    errorBanner = new ErrorBanner(this);

    queryWidget->hide();
    resultsWidget->hide();
    errorBanner->hide();

    layout->addWidget(corpusWidget);
    layout->addWidget(queryWidget);
    layout->addWidget(errorBanner);
    layout->addWidget(resultsWidget);

    mainLayout->addWidget(guiContainer, 0, Qt::AlignCenter);

    // ----------------- API -----------------
    api = new ApiManager(this);

    connect(corpusWidget, &CorpusInputWidget::corpusSelected, this, [=](const QString &dir) {
        errorBanner->hide();
        loadingBar->show();
        QUrl url(baseUrl + "/build");
        QUrlQuery query;
        query.addQueryItem("corpus_dir", dir);
        url.setQuery(query);
        api->postRequest(url);
    });

    connect(queryWidget, &QueryInputWidget::queryEntered, this, [=](const QString &q) {
        errorBanner->hide();
        loadingBar->show();
        QUrl url(baseUrl + "/search");
        QUrlQuery query;
        query.addQueryItem("query", q);
        url.setQuery(query);
        api->getRequest(url);
    });


    // ----------------- API Responses -----------------

    connect(api, &ApiManager::apiResponse, this, [=](const QJsonDocument &doc) {
        loadingBar->hide();
        QJsonObject obj = doc.object();

        if (obj.contains("message") &&
            obj["message"].toString().startsWith("Welcome to the Vector Space Model")) {
            corpusWidget->show();
            return;
        }

        if (obj.contains("error")) {
            errorBanner->showError(obj["error"].toString());
            resultsWidget->hide();
            return;
        }

        if (corpusWidget->isVisible()) {
            if (obj.contains("message") &&
                obj["message"].toString().startsWith("Index built for corpus directory:")) {
                corpusWidget->hide();
                queryWidget->show();
                queryWidget->setFocus();
            } else {
                errorBanner->showError(obj["message"].toString());
            }
        } else if (queryWidget->isVisible()) {
            resultsWidget->showResults(obj["results"].toArray());
            resultsWidget->show();
        }
    });

    connect(api, &ApiManager::apiError, this, [=](const QString &err) {
        loadingBar->hide();
        errorBanner->showError(err);
        resultsWidget->hide();
    });

    // ----------------- Hotkey -----------------
    setupHotkey();
}

void vsm::setupHotkey() {
    toggleHotkey = new QHotkey(QKeySequence("Ctrl+Space"), true, this);
    if (toggleHotkey->isRegistered())
        qDebug() << "Hotkey registered successfully";

    connect(toggleHotkey, &QHotkey::activated, this, [=]() {
        if (isVisible()) {
            hide();
            } else {
                show();
                raise();
                activateWindow();
                if (queryWidget->isVisible()) {
                    queryWidget->setFocus();
                    resultsWidget->show();
                } else {
                        corpusWidget->show();
                        corpusWidget->setFocus();
                }
            }
    });
}

void vsm::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (queryWidget->isVisible()) {
            queryWidget->hide();
            resultsWidget->hide();
            errorBanner->hide();

            corpusWidget->show();
        }
    }
    QMainWindow::keyPressEvent(event);
}

vsm::~vsm() {
    delete ui;
}
