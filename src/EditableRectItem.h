#ifndef EDITABLERECTITEM_H
#define EDITABLERECTITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>

namespace ninoan::texture_extractor {

class EditableRectItem : public QGraphicsItem {
  public:
    explicit EditableRectItem(QRectF const& rect,
                              QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option,
               QWidget* widget) override;

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

    static constexpr qreal handle_size = 10.0;
    QRectF rectangle;
    qreal scale = 1.0;
    DraggedPart currently_dragging = DraggedPart::Nothing;

    qreal scaled_handle_size() const noexcept;
};

}; // namespace ninoan::texture_extractor

#endif // EDITABLERECTITEM_H
