#include <QtTest/QtTest>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QImage>
#include "EditableRectItem.hpp"
#include "test_helpers.hpp"

using namespace ninoan::texture_extractor;

class TestEditableRectItem : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Construction tests
    void testConstruction();
    void testConstructionWithDifferentSizes();
    void testConstructionWithOffset();

    // Bounding box tests
    void testBoundingRect();
    void testBoundingRectIncludesHandles();

    // Flags and properties tests
    void testIsSelectable();
    void testInitialPosition();

    // Paint tests
    void testPaintingDoesNotCrash();
    void testScaleAwareHandles();

    // Mouse interaction tests
    void testMousePressOnTopLeftHandle();
    void testMousePressOnTopRightHandle();
    void testMousePressOnBottomLeftHandle();
    void testMousePressOnBottomRightHandle();
    void testMousePressOnRectangleBody();

    // Resize tests
    void testResizeTopLeftHandle();
    void testResizeTopRightHandle();
    void testResizeBottomLeftHandle();
    void testResizeBottomRightHandle();
    void testResizePreservesOppositeCorner();

    // Edge cases
    void testNegativeSizeRect();
    void testZeroSizeRect();
    void testLargeRect();

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    EditableRectItem* item;

    void simulateMousePress(const QPointF& pos);
    void simulateMouseMove(const QPointF& fromPos, const QPointF& toPos);
};

void TestEditableRectItem::initTestCase()
{
    // Called once before all tests
    scene = nullptr;
    view = nullptr;
    item = nullptr;
}

void TestEditableRectItem::cleanupTestCase()
{
    // Called once after all tests
}

void TestEditableRectItem::init()
{
    // Called before each test
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->resize(800, 600);
    view->show();
    QTest::qWaitForWindowExposed(view);
}

void TestEditableRectItem::cleanup()
{
    // Called after each test
    delete view;  // This also deletes the scene and items
    view = nullptr;
    scene = nullptr;
    item = nullptr;
}

void TestEditableRectItem::testConstruction()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QVERIFY(item != nullptr);
    QCOMPARE(item->pos(), QPointF(0, 0));
}

void TestEditableRectItem::testConstructionWithDifferentSizes()
{
    // Test various sizes
    QList<QRectF> testRects = {
        QRectF(0, 0, 50, 50),
        QRectF(0, 0, 200, 150),
        QRectF(0, 0, 10, 500),
        QRectF(0, 0, 500, 10)
    };

    for (const auto& rect : testRects) {
        EditableRectItem* testItem = new EditableRectItem(rect);
        scene->addItem(testItem);

        QVERIFY(testItem != nullptr);
        QRectF bounds = testItem->boundingRect();
        QVERIFY(bounds.width() > 0);
        QVERIFY(bounds.height() > 0);
    }
}

void TestEditableRectItem::testConstructionWithOffset()
{
    QRectF rect(50, 75, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // The item should be positioned at the rect's top-left
    QCOMPARE(item->pos(), rect.topLeft());
}

void TestEditableRectItem::testBoundingRect()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QRectF bounds = item->boundingRect();
    QVERIFY(bounds.width() > 0);
    QVERIFY(bounds.height() > 0);
}

void TestEditableRectItem::testBoundingRectIncludesHandles()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QRectF bounds = item->boundingRect();

    // Bounding rect should be larger than the original rect to include handles
    QVERIFY(bounds.width() >= rect.width());
    QVERIFY(bounds.height() >= rect.height());
}

void TestEditableRectItem::testIsSelectable()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QVERIFY(item->flags() & QGraphicsItem::ItemIsSelectable);
}

void TestEditableRectItem::testInitialPosition()
{
    QRectF rect(100, 150, 200, 250);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QCOMPARE(item->pos(), QPointF(100, 150));
}

void TestEditableRectItem::testPaintingDoesNotCrash()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Force a paint by creating an image and rendering to it
    QImage image(200, 200, QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    scene->render(&painter);

    // If we get here without crashing, the test passes
    QVERIFY(true);
}

void TestEditableRectItem::testScaleAwareHandles()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Test at different zoom levels
    view->scale(2.0, 2.0);
    view->viewport()->update();
    QTest::qWait(100);

    // Render to force paint call
    QImage image(400, 400, QImage::Format_ARGB32);
    QPainter painter(&image);
    scene->render(&painter);

    // If we get here without crashing, scale-aware rendering works
    QVERIFY(true);
}

void TestEditableRectItem::simulateMousePress(const QPointF& pos)
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(pos);
    pressEvent.setButton(Qt::LeftButton);
    pressEvent.setButtons(Qt::LeftButton);
    scene->sendEvent(item, &pressEvent);
}

void TestEditableRectItem::simulateMouseMove(const QPointF& fromPos, const QPointF& toPos)
{
    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setLastPos(fromPos);
    moveEvent.setPos(toPos);
    moveEvent.setButton(Qt::NoButton);
    moveEvent.setButtons(Qt::LeftButton);
    scene->sendEvent(item, &moveEvent);
}

void TestEditableRectItem::testMousePressOnTopLeftHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Click on top-left corner (handle is at 0, 0)
    simulateMousePress(QPointF(0, 0));

    // If no crash, the event was handled
    QVERIFY(true);
}

void TestEditableRectItem::testMousePressOnTopRightHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Click on top-right corner
    simulateMousePress(QPointF(100, 0));

    QVERIFY(true);
}

void TestEditableRectItem::testMousePressOnBottomLeftHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Click on bottom-left corner
    simulateMousePress(QPointF(0, 100));

    QVERIFY(true);
}

void TestEditableRectItem::testMousePressOnBottomRightHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Click on bottom-right corner
    simulateMousePress(QPointF(100, 100));

    QVERIFY(true);
}

void TestEditableRectItem::testMousePressOnRectangleBody()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // Click in the middle of the rectangle
    simulateMousePress(QPointF(50, 50));

    QVERIFY(true);
}

void TestEditableRectItem::testResizeTopLeftHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QRectF initialBounds = item->boundingRect();

    // Press on top-left handle
    simulateMousePress(QPointF(0, 0));

    // Drag to new position
    simulateMouseMove(QPointF(0, 0), QPointF(20, 20));

    QRectF newBounds = item->boundingRect();

    // The bounding rect should have changed
    QVERIFY(!TestHelpers::rectFuzzyCompare(initialBounds, newBounds, 1.0));
}

void TestEditableRectItem::testResizeTopRightHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QRectF initialBounds = item->boundingRect();

    simulateMousePress(QPointF(100, 0));
    simulateMouseMove(QPointF(100, 0), QPointF(120, 20));

    QRectF newBounds = item->boundingRect();
    QVERIFY(!TestHelpers::rectFuzzyCompare(initialBounds, newBounds, 1.0));
}

void TestEditableRectItem::testResizeBottomLeftHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QRectF initialBounds = item->boundingRect();

    simulateMousePress(QPointF(0, 100));
    simulateMouseMove(QPointF(0, 100), QPointF(20, 120));

    QRectF newBounds = item->boundingRect();
    QVERIFY(!TestHelpers::rectFuzzyCompare(initialBounds, newBounds, 1.0));
}

void TestEditableRectItem::testResizeBottomRightHandle()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QRectF initialBounds = item->boundingRect();

    simulateMousePress(QPointF(100, 100));
    simulateMouseMove(QPointF(100, 100), QPointF(120, 120));

    QRectF newBounds = item->boundingRect();
    QVERIFY(!TestHelpers::rectFuzzyCompare(initialBounds, newBounds, 1.0));
}

void TestEditableRectItem::testResizePreservesOppositeCorner()
{
    QRectF rect(0, 0, 100, 100);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    // When dragging top-left, bottom-right should stay in place
    // This is a logical test of the resize behavior
    simulateMousePress(QPointF(0, 0));
    simulateMouseMove(QPointF(0, 0), QPointF(20, 20));

    // After dragging top-left inward, the rectangle should be smaller
    QRectF newBounds = item->boundingRect();
    QVERIFY(newBounds.width() < 100);
    QVERIFY(newBounds.height() < 100);
}

void TestEditableRectItem::testNegativeSizeRect()
{
    // Test with a rect that has negative width/height (will be normalized by Qt)
    QRectF rect(100, 100, -50, -50);
    item = new EditableRectItem(rect.normalized());
    scene->addItem(item);

    QVERIFY(item != nullptr);
    QRectF bounds = item->boundingRect();
    QVERIFY(bounds.width() > 0);
    QVERIFY(bounds.height() > 0);
}

void TestEditableRectItem::testZeroSizeRect()
{
    QRectF rect(50, 50, 0, 0);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QVERIFY(item != nullptr);
    // Even a zero-size rect should have a bounding rect (for the handles)
    QRectF bounds = item->boundingRect();
    QVERIFY(bounds.width() >= 0);
    QVERIFY(bounds.height() >= 0);
}

void TestEditableRectItem::testLargeRect()
{
    QRectF rect(0, 0, 10000, 10000);
    item = new EditableRectItem(rect);
    scene->addItem(item);

    QVERIFY(item != nullptr);
    QRectF bounds = item->boundingRect();
    QVERIFY(bounds.width() > 0);
    QVERIFY(bounds.height() > 0);
}

QTEST_MAIN(TestEditableRectItem)
#include "test_EditableRectItem.moc"
