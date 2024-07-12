#include "documentwindow.h"
#include "EditableRectItem.h"
#include "ExtractedView.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneDragDropEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

using EditableRectItem = ninoan::texture_extractor::EditableRectItem;

class MovableEllipse : public QGraphicsEllipseItem {
  public:
    explicit MovableEllipse(const QRectF& rect, QGraphicsItem* parent = nullptr)
        : QGraphicsEllipseItem{rect, parent} {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::AllButtons);
        setCursor(Qt::OpenHandCursor);
    }

  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent*) override {
        setCursor(Qt::OpenHandCursor);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override {
        setCursor(Qt::ArrowCursor);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent*) override {
        setCursor(Qt::ClosedHandCursor);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override {
        setCursor(Qt::OpenHandCursor);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        auto origin = event->lastScenePos();
        auto event_pos = event->scenePos();
        setPos(this->pos() + event_pos - origin);
    }
};

PhotoView::PhotoView(QString const& file_path, QWidget* parent)
    : QWidget{parent}, image(QImage(file_path)) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    graphics = new SourceImageView(this);
    extracted_view = new ExtractedView(this);

    layout->addWidget(extracted_view);
    layout->addWidget(graphics);

    QGraphicsScene* scene = new QGraphicsScene(graphics);
    graphics->setScene(scene);
    auto rect = QRect(10, 10, 20, 20);
    auto ellipse = new MovableEllipse(rect, nullptr);
    scene->addItem(ellipse);
    ellipse->setZValue(1.0);

    auto editable_rect = new EditableRectItem(rect, nullptr);
    scene->addItem(editable_rect);
    editable_rect->setZValue(2.0);

    setLayout(layout);

    show_image(file_path);
}

void PhotoView::show_image(QString path) {
    QGraphicsScene* scene = graphics->scene();
    image = QImage(path);
    if (!image.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(image);
        QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
        item->setScale(0.1);
    }
}

DocumentWindow::DocumentWindow(QMainWindow* parent) : QMainWindow{parent} {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("New Document");
    auto loading_label = new QLabel("Loading...", this);
    this->setCentralWidget(loading_label);
}

void DocumentWindow::set_document_title(const QString& new_title) {
    qDebug() << "The new window title is " << new_title << "\n";
    document_title = new_title;
    this->setWindowTitle(new_title);
    photo_view = new PhotoView(new_title, this);
    this->setCentralWidget(photo_view);
    /* photo_view->show_image(new_title); */
}

QString DocumentWindow::get_document_title() { return document_title; }
