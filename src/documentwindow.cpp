#include "documentwindow.hpp"
#include "EditableRectItem.hpp"
#include "ExtractedView.hpp"
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
        this->setAcceptHoverEvents(true);
        this->setAcceptedMouseButtons(Qt::AllButtons);
        this->setCursor(Qt::OpenHandCursor);
    }

  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent*) override {
        this->setCursor(Qt::OpenHandCursor);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override {
        this->setCursor(Qt::ArrowCursor);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent*) override {
        this->setCursor(Qt::ClosedHandCursor);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override {
        this->setCursor(Qt::OpenHandCursor);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        auto origin = event->lastScenePos();
        auto eventPos = event->scenePos();
        this->setPos(this->pos() + eventPos - origin);
    }
};

PhotoView::PhotoView(QString const& filePath, QWidget* parent)
    : QWidget{parent}, image(QImage(filePath)) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    this->graphics = new SourceImageView(this);
    this->extractedView = new ExtractedView(this);

    layout->addWidget(this->extractedView);
    layout->addWidget(this->graphics);

    QGraphicsScene* scene = new QGraphicsScene(this->graphics);
    this->graphics->setScene(scene);
    auto rect = QRect(10, 10, 20, 20);
    auto ellipse = new MovableEllipse(rect, nullptr);
    scene->addItem(ellipse);
    ellipse->setZValue(1.0);

    auto editableRect = new EditableRectItem(rect, nullptr);
    scene->addItem(editableRect);
    editableRect->setZValue(2.0);

    this->setLayout(layout);

    this->showImage(filePath);
}

void PhotoView::showImage(QString path) {
    QGraphicsScene* scene = this->graphics->scene();
    this->image = QImage(path);
    if (!this->image.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(this->image);
        QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
        item->setScale(0.1);
    }
}

DocumentWindow::DocumentWindow(QMainWindow* parent) : QMainWindow{parent} {
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("New Document");
    auto loading_label = new QLabel("Loading...", this);
    this->setCentralWidget(loading_label);
}

void DocumentWindow::setDocumentTitle(const QString& newTitle) {
    qDebug() << "The new window title is " << newTitle << "\n";
    this->documentTitle = newTitle;
    this->setWindowTitle(newTitle);
    this->photoView = new PhotoView(newTitle, this);
    this->setCentralWidget(this->photoView);
    /* this->photoView->showImage(newTitle); */
}

QString DocumentWindow::getDocumentTitle() { return this->documentTitle; }
