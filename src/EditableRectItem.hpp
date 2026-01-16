#pragma once

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>

namespace ninoan::texture_extractor {

class EditableRectItem : public QGraphicsItem {
  public:
    explicit EditableRectItem(QRectF const& rect, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) override;

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

  private:
    enum class DraggedPart {
        Nothing,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        WholeRectangle,
    };

    static constexpr qreal handleSize = 10.0;
    QRectF rectangle;
    qreal scale = 1.0;
    DraggedPart currentlyDragging = DraggedPart::Nothing;

    qreal scaledHandleSize() const noexcept;
};

} // namespace ninoan::texture_extractor
