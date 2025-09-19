#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QLineEdit>
#include <QString>

class CorpusInputWidget;  // forward declarations
class QueryInputWidget;

class SearchBar : public QLineEdit {
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);

signals:
    void searchRequested(const QString &text);

private:
    CorpusInputWidget *corpusWidget = nullptr;
    QueryInputWidget *queryWidget = nullptr;
};

#endif // SEARCHBAR_H
