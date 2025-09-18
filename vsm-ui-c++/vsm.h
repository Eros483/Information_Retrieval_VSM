#ifndef VSM_H
#define VSM_H

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QPushButton>
#include "SearchBar.h"
#include "ApiManager.h"
#include <QHotkey>

QT_BEGIN_NAMESPACE
namespace Ui { class vsm; }
QT_END_NAMESPACE

class vsm : public QMainWindow
{
    Q_OBJECT

public:
    vsm(QWidget *parent = nullptr);
    ~vsm();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::vsm *ui;

    SearchBar *corpusBar;
    SearchBar *queryBar;
    QLabel *errorLabel;
    QListWidget *resultsList;
    QHotkey *toggleHotkey;
    ApiManager *api;

    QLabel *corpusLabel;
    QLabel *queryLabel;

    QPushButton *browseButton;
};

#endif // VSM_H
