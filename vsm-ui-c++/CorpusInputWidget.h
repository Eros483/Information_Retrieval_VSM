#ifndef CORPUSINPUTWIDGET_H
#define CORPUSINPUTWIDGET_H

#include <QWidget>

class SearchBar;
class QPushButton;

class CorpusInputWidget : public QWidget {
    Q_OBJECT
public:
    explicit CorpusInputWidget(QWidget *parent = nullptr);
    QString corpusPath() const;
    void setBrowseVisible(bool visible);

signals:
    void corpusSelected(const QString &dir);

private:
    SearchBar *corpusBar;
    QPushButton *browseButton;
};

#endif // CORPUSINPUTWIDGET_H
