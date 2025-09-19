#ifndef VSM_H
#define VSM_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QHotkey>
#include "ApiManager.h"
#include <QProgressBar>
// new modular widgets
#include "CorpusInputWidget.h"
#include "QueryInputWidget.h"
#include "ResultsListWidget.h"
#include "ErrorBanner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class vsm; }
QT_END_NAMESPACE

class vsm : public QMainWindow
{
    Q_OBJECT

public:
    explicit vsm(QWidget *parent = nullptr);
    ~vsm();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::vsm *ui;

    QString baseUrl = "http://localhost:8000";

    QProgressBar *loadingBar;
    CorpusInputWidget *corpusWidget;
    QueryInputWidget *queryWidget;
    ResultsListWidget *resultsWidget;
    ErrorBanner *errorBanner;

    QHotkey *toggleHotkey;
    ApiManager *api;

    void setupHotkey();
};

#endif // VSM_H
