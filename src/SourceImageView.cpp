#include "SourceImageView.h"
#include <QDebug>
#include <QGraphicsView>
#include <QMouseEvent>

SourceImageView::SourceImageView(QWidget* parent) : QGraphicsView{parent} {
    setDragMode(QGraphicsView::NoDrag);
}

void SourceImageView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        // Start custom drag
        setDragMode(QGraphicsView::ScrollHandDrag);
        // Simulate a left button press to start the drag
        QMouseEvent fakeEvent(event->type(), event->position(),
                              event->globalPosition(), Qt::LeftButton,
                              Qt::LeftButton, event->modifiers(),
                              event->pointingDevice());
        QGraphicsView::mousePressEvent(&fakeEvent);
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void SourceImageView::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::RightButton) {
        // Continue custom drag
        QMouseEvent fakeEvent(event->type(), event->position(),
                              event->globalPosition(), Qt::LeftButton,
                              Qt::LeftButton, event->modifiers(),
                              event->pointingDevice());
        QGraphicsView::mouseMoveEvent(&fakeEvent);
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void SourceImageView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        // End custom drag
        setDragMode(QGraphicsView::NoDrag);
        // Simulate a left button release to end the drag
        QMouseEvent fakeEvent(event->type(), event->position(),
                              event->globalPosition(), Qt::LeftButton,
                              Qt::LeftButton, event->modifiers(),
                              event->pointingDevice());
        QGraphicsView::mouseReleaseEvent(&fakeEvent);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void SourceImageView::wheelEvent(QWheelEvent* event) {
    qDebug() << event->pixelDelta();
    auto pixel_delta = event->pixelDelta().y() * 0.01;
    auto sign = pixel_delta >= 0 ? 1 : -1;
    auto delta = std::abs(1.0 + sign * std::sqrt(std::abs(pixel_delta)));
    scale(delta, delta);
}
