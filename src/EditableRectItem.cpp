#include "EditableRectItem.h"
#include <QRectF>

namespace ninoan::texture_extractor {

EditableRectItem::EditableRectItem(QRectF const& rect, QGraphicsItem* parent)
    : QGraphicsItem{parent} {
    rectangle = QRectF(QPoint(0, 0), rect.size());
    setPos(rect.topLeft());
    setFlags(QGraphicsItem::ItemIsSelectable);
}

QRectF EditableRectItem::boundingRect() const {
    // Add some padding for the handles
    return rectangle.adjusted(
        (-handle_size / 2) / scale, (-handle_size / 2) / scale,
        (handle_size / 2) / scale, (handle_size / 2) / scale);
}

qreal EditableRectItem::scaled_handle_size() const noexcept {
    return handle_size / scale;
}

void EditableRectItem::paint(QPainter* painter,
                             QStyleOptionGraphicsItem const* option,
                             QWidget* widget) {
    Q_UNUSED(widget);

    // Get the current view scale
    scale = option->levelOfDetailFromTransform(painter->worldTransform());

    // Set a constant visual size for pen and handles
    qreal penWidth = 3.0 / scale;

    // Draw the rectangle
    painter->setPen(QPen(Qt::blue, penWidth));
    painter->drawRect(rectangle);

    // Draw the handles
    painter->setBrush(Qt::red);
    painter->setPen(Qt::NoPen);

    qreal scaled_handle = scaled_handle_size();
    painter->drawRect(QRectF(rectangle.left() - scaled_handle / 2,
                             rectangle.top() - scaled_handle / 2, scaled_handle,
                             scaled_handle));
    painter->drawRect(QRectF(rectangle.left() - scaled_handle / 2,
                             rectangle.bottom() - scaled_handle / 2,
                             scaled_handle, scaled_handle));
    painter->drawRect(QRectF(rectangle.right() - scaled_handle / 2,
                             rectangle.top() - scaled_handle / 2, scaled_handle,
                             scaled_handle));
    painter->drawRect(QRectF(rectangle.right() - scaled_handle / 2,
                             rectangle.bottom() - scaled_handle / 2,
                             scaled_handle, scaled_handle));
}

void EditableRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {

    qreal scaled_handle = scaled_handle_size();
    auto top_left = QRectF(rectangle.left() - scaled_handle / 2,
                           rectangle.top() - scaled_handle / 2, scaled_handle,
                           scaled_handle);
    auto bottom_left = QRectF(rectangle.left() - scaled_handle / 2,
                              rectangle.bottom() - scaled_handle / 2,
                              scaled_handle, scaled_handle);
    auto top_right = QRectF(rectangle.right() - scaled_handle / 2,
                            rectangle.top() - scaled_handle / 2, scaled_handle,
                            scaled_handle);
    auto bottom_right = QRectF(rectangle.right() - scaled_handle / 2,
                               rectangle.bottom() - scaled_handle / 2,
                               scaled_handle, scaled_handle);

    if (top_left.contains(event->pos())) {
        currently_dragging = EditableRectItem::DraggedPart::TopLeft;
    } else if (bottom_left.contains(event->pos())) {
        currently_dragging = EditableRectItem::DraggedPart::BottomLeft;
    } else if (top_right.contains(event->pos())) {
        currently_dragging = EditableRectItem::DraggedPart::TopRight;
    } else if (bottom_right.contains(event->pos())) {
        currently_dragging = EditableRectItem::DraggedPart::BottomRight;
    } else {
        currently_dragging = EditableRectItem::DraggedPart::WholeRectangle;
    }
}

void EditableRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    auto delta = event->pos() - event->lastPos();
    switch (currently_dragging) {
    case EditableRectItem::DraggedPart::TopLeft: {
        rectangle.setTopLeft(rectangle.topLeft() + delta);
        break;
    }
    case EditableRectItem::DraggedPart::TopRight: {
        rectangle.setTopRight(rectangle.topRight() + delta);
        break;
    }
    case EditableRectItem::DraggedPart::BottomLeft: {
        rectangle.setBottomLeft(rectangle.bottomLeft() + delta);
        break;
    }
    case EditableRectItem::DraggedPart::BottomRight: {
        rectangle.setBottomRight(rectangle.bottomRight() + delta);
        break;
    }
    default: {
    }
    }
    this->update();
}

}; // namespace ninoan::texture_extractor
