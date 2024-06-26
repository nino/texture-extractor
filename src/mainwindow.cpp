#include "mainwindow.h"
#include "documentwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QMenuBar>
#include <QMimeData>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDragEnterEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent} {
    openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                             tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

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
            doc->set_document_title(fileName);
            doc->show();
            qDebug() << doc->get_document_title();
        }
    }
}

void MainWindow::openFile(const QString& fileName) noexcept {
    DocumentWindow* doc = new DocumentWindow(this);
    doc->set_document_title(fileName);
    doc->show();
    qDebug() << "Opened file:" << doc->get_document_title();
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
                openFile(url.toLocalFile());
            }
        }
    }
}
