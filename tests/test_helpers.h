#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <QImage>
#include <QPixmap>
#include <QString>
#include <QTemporaryFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QPointF>

namespace TestHelpers {

/**
 * Creates a test image with specified dimensions and color
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param color Fill color (default: white)
 * @return QImage object
 */
QImage createTestImage(int width, int height, const QColor& color = Qt::white);

/**
 * Creates a temporary image file for testing file loading
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param format File format (e.g., "PNG", "JPEG")
 * @return Path to temporary file (caller must manage lifetime)
 */
QString createTempImageFile(int width, int height, const QString& format = "PNG");

/**
 * Simulates a mouse press event
 * @param widget Target widget
 * @param button Mouse button
 * @param pos Position in widget coordinates
 * @param globalPos Global position
 */
void simulateMousePress(QWidget* widget, Qt::MouseButton button,
                       const QPoint& pos, const QPoint& globalPos);

/**
 * Simulates a mouse move event
 * @param widget Target widget
 * @param pos Position in widget coordinates
 * @param globalPos Global position
 * @param buttons Currently pressed buttons
 */
void simulateMouseMove(QWidget* widget, const QPoint& pos,
                      const QPoint& globalPos, Qt::MouseButtons buttons);

/**
 * Simulates a mouse release event
 * @param widget Target widget
 * @param button Mouse button
 * @param pos Position in widget coordinates
 * @param globalPos Global position
 */
void simulateMouseRelease(QWidget* widget, Qt::MouseButton button,
                         const QPoint& pos, const QPoint& globalPos);

/**
 * Simulates a mouse wheel event
 * @param widget Target widget
 * @param delta Wheel delta (positive = up, negative = down)
 * @param pos Position in widget coordinates
 * @param globalPos Global position
 */
void simulateWheelEvent(QWidget* widget, int delta,
                       const QPoint& pos, const QPoint& globalPos);

/**
 * Simulates a click and drag operation
 * @param widget Target widget
 * @param button Mouse button to use
 * @param from Starting position
 * @param to Ending position
 */
void simulateDrag(QWidget* widget, Qt::MouseButton button,
                 const QPoint& from, const QPoint& to);

/**
 * Compares two QRectF objects with a tolerance
 * @param rect1 First rectangle
 * @param rect2 Second rectangle
 * @param tolerance Allowed difference (default: 0.01)
 * @return true if rectangles are approximately equal
 */
bool rectFuzzyCompare(const QRectF& rect1, const QRectF& rect2, qreal tolerance = 0.01);

/**
 * Compares two QPointF objects with a tolerance
 * @param point1 First point
 * @param point2 Second point
 * @param tolerance Allowed difference (default: 0.01)
 * @return true if points are approximately equal
 */
bool pointFuzzyCompare(const QPointF& point1, const QPointF& point2, qreal tolerance = 0.01);

/**
 * Gets a graphics item at a specific scene position
 * @param scene Graphics scene
 * @param scenePos Position in scene coordinates
 * @return Item at position, or nullptr if none
 */
QGraphicsItem* itemAtScenePos(QGraphicsScene* scene, const QPointF& scenePos);

} // namespace TestHelpers

#endif // TEST_HELPERS_H
