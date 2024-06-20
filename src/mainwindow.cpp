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
    openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                             tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
}

MainWindow::~MainWindow() {}

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
            doc->set_document_title(fileName);
            doc->show();
            qDebug() << doc->get_document_title();
        }
    }
}
