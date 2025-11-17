#include "test_helpers.h"
#include <QPainter>
#include <QApplication>
#include <QtMath>
#include <QTemporaryFile>

namespace TestHelpers {

QImage createTestImage(int width, int height, const QColor& color)
{
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(color);

    // Add a simple pattern to make the image more identifiable
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(10, 10, width - 20, height - 20);
    painter.drawLine(0, 0, width, height);
    painter.drawLine(width, 0, 0, height);

    return image;
}

QString createTempImageFile(int width, int height, const QString& format)
{
    QImage image = createTestImage(width, height);

    QString suffix = format.toLower();
    if (!suffix.startsWith('.')) {
        suffix = "." + suffix;
    }

    QTemporaryFile* tempFile = new QTemporaryFile(
        QDir::tempPath() + "/test_image_XXXXXX" + suffix);
    tempFile->setAutoRemove(false); // Caller manages lifetime

    if (tempFile->open()) {
        QString filePath = tempFile->fileName();
        tempFile->close();

        if (image.save(filePath, format.toUtf8().constData())) {
            return filePath;
        }
    }

    delete tempFile;
    return QString();
}

void simulateMousePress(QWidget* widget, Qt::MouseButton button,
                       const QPoint& pos, const QPoint& globalPos)
{
    QMouseEvent event(QEvent::MouseButtonPress, pos, globalPos, button,
                     button, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

void simulateMouseMove(QWidget* widget, const QPoint& pos,
                      const QPoint& globalPos, Qt::MouseButtons buttons)
{
    QMouseEvent event(QEvent::MouseMove, pos, globalPos, Qt::NoButton,
                     buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

void simulateMouseRelease(QWidget* widget, Qt::MouseButton button,
                         const QPoint& pos, const QPoint& globalPos)
{
    QMouseEvent event(QEvent::MouseButtonRelease, pos, globalPos, button,
                     Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

void simulateWheelEvent(QWidget* widget, int delta,
                       const QPoint& pos, const QPoint& globalPos)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    QWheelEvent event(pos, globalPos, QPoint(), QPoint(0, delta),
                     Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
#else
    QWheelEvent event(pos, globalPos, delta, Qt::NoButton, Qt::NoModifier);
#endif
    QApplication::sendEvent(widget, &event);
}

void simulateDrag(QWidget* widget, Qt::MouseButton button,
                 const QPoint& from, const QPoint& to)
{
    // Press at start position
    simulateMousePress(widget, button, from, widget->mapToGlobal(from));

    // Move in steps to simulate realistic drag
    int steps = 10;
    for (int i = 1; i <= steps; ++i) {
        QPoint pos = from + (to - from) * i / steps;
        QPoint globalPos = widget->mapToGlobal(pos);
        simulateMouseMove(widget, pos, globalPos, button);
        QApplication::processEvents();
    }

    // Release at end position
    simulateMouseRelease(widget, button, to, widget->mapToGlobal(to));
    QApplication::processEvents();
}

bool rectFuzzyCompare(const QRectF& rect1, const QRectF& rect2, qreal tolerance)
{
    return qAbs(rect1.x() - rect2.x()) <= tolerance &&
           qAbs(rect1.y() - rect2.y()) <= tolerance &&
           qAbs(rect1.width() - rect2.width()) <= tolerance &&
           qAbs(rect1.height() - rect2.height()) <= tolerance;
}

bool pointFuzzyCompare(const QPointF& point1, const QPointF& point2, qreal tolerance)
{
    return qAbs(point1.x() - point2.x()) <= tolerance &&
           qAbs(point1.y() - point2.y()) <= tolerance;
}

QGraphicsItem* itemAtScenePos(QGraphicsScene* scene, const QPointF& scenePos)
{
    QList<QGraphicsItem*> items = scene->items(scenePos, Qt::IntersectsItemShape,
                                                Qt::DescendingOrder);
    return items.isEmpty() ? nullptr : items.first();
}

} // namespace TestHelpers
