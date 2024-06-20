#include "documentwindow.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>

PhotoView::PhotoView(QWidget* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* lab1 = new QLabel("hello", this);
    graphics = new QGraphicsView(this);
    layout->addWidget(lab1);
    layout->addWidget(graphics);

    QGraphicsScene* scene = new QGraphicsScene(graphics);
    graphics->setScene(scene);
    QRect rect(10, 10, 20, 20);
    scene->addEllipse(rect);

    setLayout(layout);
}

void PhotoView::show_image(QString path) {
    QGraphicsScene* scene = graphics->scene();
    QImage img(path);
    if (!img.isNull()) {
        QGraphicsPixmapItem* pixmap_item =
            scene->addPixmap(QPixmap::fromImage(img));
        scene->setSceneRect(pixmap_item->boundingRect());
    }
}

DocumentWindow::DocumentWindow(QMainWindow* parent) : QMainWindow{parent} {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("New Document");
    photo_view = new PhotoView(this);
    this->setCentralWidget(photo_view);
}

void DocumentWindow::set_document_title(const QString& new_title) {
    qDebug() << "The new window title is " << new_title << "\n";
    document_title = new_title;
    this->setWindowTitle(new_title);
    photo_view->show_image(new_title);
}

QString DocumentWindow::get_document_title() { return document_title; }
