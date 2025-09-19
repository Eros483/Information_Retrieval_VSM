#include "ErrorBanner.h"

ErrorBanner::ErrorBanner(QWidget *parent) : QLabel(parent) {
    setStyleSheet("QLabel { background-color: rgba(255,100,100,180); color: white; border-radius: 15px; padding: 8px; }");
    setAlignment(Qt::AlignCenter);
    hide();
}

void ErrorBanner::showError(const QString &msg) {
    setText(msg);
    show();
}
