#include "documentwindow.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneDragDropEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class MovableEllipse : public QGraphicsEllipseItem {
  public:
    explicit MovableEllipse(const QRectF& rect, QGraphicsItem* parent = nullptr)
        : QGraphicsEllipseItem{rect, parent} {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::AllButtons);
        setCursor(Qt::OpenHandCursor);
    }

  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        setCursor(Qt::OpenHandCursor);
        QGraphicsEllipseItem::hoverEnterEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        setCursor(Qt::ArrowCursor);
        QGraphicsEllipseItem::hoverLeaveEvent(event);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        setCursor(Qt::ClosedHandCursor);
        QGraphicsEllipseItem::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        setCursor(Qt::OpenHandCursor);
        QGraphicsEllipseItem::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        auto origin = event->lastScenePos();
        auto event_pos = event->scenePos();
        setPos(this->pos() + event_pos - origin);
        QGraphicsEllipseItem::mouseMoveEvent(event);
    }
};

PhotoView::PhotoView(QWidget* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* lab1 = new QLabel("hello", this);
    graphics = new QGraphicsView(this);
    /* graphics->setDragMode(QGraphicsView::ScrollHandDrag); */

    layout->addWidget(lab1);
    layout->addWidget(graphics);

    QPushButton* zoom_in_button = new QPushButton("+", this);
    QPushButton* zoom_out_button = new QPushButton("−", this);
    connect(zoom_in_button, &QPushButton::clicked, this, &PhotoView::zoom_in);
    connect(zoom_out_button, &QPushButton::clicked, this, &PhotoView::zoom_out);

    layout->addWidget(zoom_in_button);
    layout->addWidget(zoom_out_button);

    QGraphicsScene* scene = new QGraphicsScene(graphics);
    graphics->setScene(scene);
    auto rect = QRect(10, 10, 20, 20);
    auto ellipse = new MovableEllipse(rect, nullptr);
    scene->addItem(ellipse);
    ellipse->setZValue(1.0);

    setLayout(layout);
}

void PhotoView::show_image(QString path) {
    QGraphicsScene* scene = graphics->scene();
    QImage img(path);
    if (!img.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(img);
        QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
        item->setScale(0.1);
    }
}

void PhotoView::zoom_in() {
    qDebug() << "zooming in";
    graphics->scale(1.1, 1.1);
}

void PhotoView::zoom_out() {
    qDebug() << "zooming out";
    graphics->scale(0.9, 0.9);
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
