#include "mainwindow.h"
#include "documentwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent} {
    documents = new QList<DocumentWindow*>();

    openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                             tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
}

MainWindow::~MainWindow() { delete documents; }

void MainWindow::open() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("*.png *.jpg *.jpeg *.tif *.tiff");
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        qDebug() << fileNames;
        for (auto fileName : fileNames) {
            DocumentWindow* doc = new DocumentWindow(this);
            documents->append(doc);
            doc->set_document_title(fileName);
            doc->show();
        }
        for (auto doc : *documents) {
            qDebug() << doc->get_document_title();
        }
    }
}
