#include "mainwindow.hpp"
#include "documentwindow.hpp"
#include <QDebug>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QMenuBar>
#include <QMimeData>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent} {
    this->openAction =
        new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
    this->openAction->setShortcuts(QKeySequence::Open);
    this->openAction->setStatusTip(tr("Open a file"));
    connect(this->openAction, &QAction::triggered, this, &MainWindow::open);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
    setAcceptDrops(true);
}

MainWindow::~MainWindow() {}

void MainWindow::open() noexcept {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("*.png *.jpg *.jpeg *.tif *.tiff");
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        qDebug() << fileNames;
        for (auto fileName : fileNames) {
            DocumentWindow* doc = new DocumentWindow(this);
            doc->setDocumentTitle(fileName);
            doc->show();
            qDebug() << doc->getDocumentTitle();
        }
    }
}

void MainWindow::openFile(const QString& fileName) noexcept {
    DocumentWindow* doc = new DocumentWindow(this);
    doc->setDocumentTitle(fileName);
    doc->show();
    qDebug() << "Opened file:" << doc->getDocumentTitle();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        for (const QUrl& url : urlList) {
            if (url.isLocalFile()) {
                this->openFile(url.toLocalFile());
            }
        }
    }
}
