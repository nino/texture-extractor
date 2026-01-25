#include "EditableRectItem.hpp"
#include <QRectF>

namespace ninoan::texture_extractor {

EditableRectItem::EditableRectItem(QRectF const& rect, QGraphicsItem* parent)
    : QGraphicsItem{parent} {
   this->rectangle = QRectF(QPoint(0, 0), rect.size());
   this->setPos(rect.topLeft());
   this->setFlags(QGraphicsItem::ItemIsSelectable);
}

QRectF EditableRectItem::boundingRect() const {
   // Add some padding for the handles
   return this->rectangle.adjusted(
       (-handleSize / 2) / this->scale, (-handleSize / 2) / this->scale,
       (handleSize / 2) / this->scale, (handleSize / 2) / this->scale);
}

qreal EditableRectItem::scaledHandleSize() const noexcept {
   return handleSize / this->scale;
}

void EditableRectItem::paint(QPainter* painter,
                             QStyleOptionGraphicsItem const* option,
                             QWidget* widget) {
   Q_UNUSED(widget);

   // Get the current view scale
   this->scale = option->levelOfDetailFromTransform(painter->worldTransform());

   // Set a constant visual size for pen and handles
   qreal penWidth = 3.0 / this->scale;

   // Draw the rectangle
   painter->setPen(QPen(Qt::blue, penWidth));
   painter->drawRect(this->rectangle);

   // Draw the handles
   painter->setBrush(Qt::red);
   painter->setPen(Qt::NoPen);

   qreal scaledHandle = this->scaledHandleSize();
   painter->drawRect(QRectF(this->rectangle.left() - scaledHandle / 2,
                            this->rectangle.top() - scaledHandle / 2,
                            scaledHandle, scaledHandle));
   painter->drawRect(QRectF(this->rectangle.left() - scaledHandle / 2,
                            this->rectangle.bottom() - scaledHandle / 2,
                            scaledHandle, scaledHandle));
   painter->drawRect(QRectF(this->rectangle.right() - scaledHandle / 2,
                            this->rectangle.top() - scaledHandle / 2,
                            scaledHandle, scaledHandle));
   painter->drawRect(QRectF(this->rectangle.right() - scaledHandle / 2,
                            this->rectangle.bottom() - scaledHandle / 2,
                            scaledHandle, scaledHandle));
}

void EditableRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {

   qreal scaledHandle = this->scaledHandleSize();
   auto topLeft = QRectF(this->rectangle.left() - scaledHandle / 2,
                         this->rectangle.top() - scaledHandle / 2, scaledHandle,
                         scaledHandle);
   auto bottomLeft = QRectF(this->rectangle.left() - scaledHandle / 2,
                            this->rectangle.bottom() - scaledHandle / 2,
                            scaledHandle, scaledHandle);
   auto topRight = QRectF(this->rectangle.right() - scaledHandle / 2,
                          this->rectangle.top() - scaledHandle / 2,
                          scaledHandle, scaledHandle);
   auto bottomRight = QRectF(this->rectangle.right() - scaledHandle / 2,
                             this->rectangle.bottom() - scaledHandle / 2,
                             scaledHandle, scaledHandle);

   if (topLeft.contains(event->pos())) {
      this->currentlyDragging = EditableRectItem::DraggedPart::TopLeft;
   } else if (bottomLeft.contains(event->pos())) {
      this->currentlyDragging = EditableRectItem::DraggedPart::BottomLeft;
   } else if (topRight.contains(event->pos())) {
      this->currentlyDragging = EditableRectItem::DraggedPart::TopRight;
   } else if (bottomRight.contains(event->pos())) {
      this->currentlyDragging = EditableRectItem::DraggedPart::BottomRight;
   } else {
      this->currentlyDragging = EditableRectItem::DraggedPart::WholeRectangle;
   }
}

void EditableRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
   auto delta = event->pos() - event->lastPos();
   switch (this->currentlyDragging) {
   case EditableRectItem::DraggedPart::TopLeft: {
      this->rectangle.setTopLeft(this->rectangle.topLeft() + delta);
      break;
   }
   case EditableRectItem::DraggedPart::TopRight: {
      this->rectangle.setTopRight(this->rectangle.topRight() + delta);
      break;
   }
   case EditableRectItem::DraggedPart::BottomLeft: {
      this->rectangle.setBottomLeft(this->rectangle.bottomLeft() + delta);
      break;
   }
   case EditableRectItem::DraggedPart::BottomRight: {
      this->rectangle.setBottomRight(this->rectangle.bottomRight() + delta);
      break;
   }
   default: {
   }
   }
   this->update();
}

}; // namespace ninoan::texture_extractor
