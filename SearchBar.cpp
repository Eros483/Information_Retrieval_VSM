#include "SearchBar.h"

SearchBar::SearchBar(QWidget *parent) : QLineEdit(parent) {
    setPlaceholderText("Search...");
    setFixedHeight(40);
    setFixedWidth(400);
    setStyleSheet(
        "QLineEdit {"
        "  background-color: rgba(255, 255, 255, 80);"
        "  border: 2px solid rgba(255, 255, 255, 120);"
        "  border-radius: 20px;"
        "  padding-left: 15px;"
        "  color: black;"
        "  font-size: 16px;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid rgba(255, 255, 255, 200);"
        "}"
        );

    connect(this, &QLineEdit::returnPressed, this, [=](){
        emit searchRequested(text());
    });
}
