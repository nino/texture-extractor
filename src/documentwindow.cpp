#include "documentwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>

DocumentWindow::DocumentWindow(QMainWindow* parent) : QMainWindow{parent} {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("New Document");
}

void DocumentWindow::set_document_title(const QString& new_title) {
    qDebug() << "The new window title is " << new_title << "\n";
    document_title = new_title;
    this->setWindowTitle(new_title);
    QLabel* lab = new QLabel("hello", this);
    QLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lab);
    this->setLayout(layout);
}

QString DocumentWindow::get_document_title() { return document_title; }
