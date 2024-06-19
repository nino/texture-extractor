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
    /* QWidget *central = new QWidget(this); */
    /* QLabel *lab = new QLabel("Hello", this); */
    /* QPushButton *butt = new QPushButton("Click me", this); */
    /* QGraphicsView *graphics = new QGraphicsView(this); */
    /* QVBoxLayout *layout = new QVBoxLayout(this); */
    /* layout->addWidget(lab); */
    /* layout->addWidget(butt); */
    /* layout->addWidget(graphics); */
    /* central->setLayout(layout); */
    /* this->setWindowTitle("what"); */
    /* this->setCentralWidget(central); */

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
    dialog.setFileMode(QFileDialog::AnyFile);
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
