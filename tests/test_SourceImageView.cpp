#include "SourceImageView.hpp"
#include "test_helpers.hpp"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPointF>
#include <QWheelEvent>
#include <QtTest/QtTest>

class TestSourceImageView : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Construction tests
    void testConstruction();
    void testInitialDragMode();

    // Mouse event tests
    void testLeftClickPassthrough();
    void testRightClickStartsDrag();
    void testRightClickChangesDragMode();
    void testRightClickReleasesRestoresDragMode();
    void testRightClickDragSequence();

    // Wheel event tests
    void testWheelZoomIn();
    void testWheelZoomOut();
    void testMultipleZoomOperations();
    void testZoomChangesTransform();

    // Scene interaction tests
    void testWithScene();
    void testWithSceneAndContent();
    void testPanningWithContent();

  private:
    SourceImageView* view;
    QGraphicsScene* scene;
    QTransform getViewTransform();
};

void TestSourceImageView::initTestCase() {
    view = nullptr;
    scene = nullptr;
}

void TestSourceImageView::cleanupTestCase() {
    // Called after all tests
}

void TestSourceImageView::init() {
    // Create fresh instances for each test
    view = new SourceImageView();
    scene = new QGraphicsScene();
    view->setScene(scene);
    view->resize(800, 600);
    view->show();
    QTest::qWaitForWindowExposed(view);
}

void TestSourceImageView::cleanup() {
    delete view;
    view = nullptr;
    delete scene;
    scene = nullptr;
}

QTransform TestSourceImageView::getViewTransform() { return view->transform(); }

void TestSourceImageView::testConstruction() {
    QVERIFY(view != nullptr);
    QVERIFY(view->scene() == scene);
}

void TestSourceImageView::testInitialDragMode() {
    QCOMPARE(view->dragMode(), QGraphicsView::NoDrag);
}

void TestSourceImageView::testLeftClickPassthrough() {
    // Left click should not change drag mode
    QPointF scenePos(100, 100);
    QPoint viewPos = view->mapFromScene(scenePos);

    TestHelpers::simulateMousePress(view->viewport(), Qt::LeftButton, viewPos,
                                    view->viewport()->mapToGlobal(viewPos));

    // Drag mode should still be NoDrag
    QCOMPARE(view->dragMode(), QGraphicsView::NoDrag);

    TestHelpers::simulateMouseRelease(view->viewport(), Qt::LeftButton, viewPos,
                                      view->viewport()->mapToGlobal(viewPos));
}

void TestSourceImageView::testRightClickStartsDrag() {
    QPointF scenePos(100, 100);
    QPoint viewPos = view->mapFromScene(scenePos);

    // Initial drag mode should be NoDrag
    QCOMPARE(view->dragMode(), QGraphicsView::NoDrag);

    // Right click should change drag mode
    TestHelpers::simulateMousePress(view->viewport(), Qt::RightButton, viewPos,
                                    view->viewport()->mapToGlobal(viewPos));

    // Drag mode should now be ScrollHandDrag
    QCOMPARE(view->dragMode(), QGraphicsView::ScrollHandDrag);

    // Clean up
    TestHelpers::simulateMouseRelease(view->viewport(), Qt::RightButton,
                                      viewPos,
                                      view->viewport()->mapToGlobal(viewPos));
}

void TestSourceImageView::testRightClickChangesDragMode() {
    QCOMPARE(view->dragMode(), QGraphicsView::NoDrag);

    QPoint pos(100, 100);
    TestHelpers::simulateMousePress(view->viewport(), Qt::RightButton, pos,
                                    view->viewport()->mapToGlobal(pos));

    QCOMPARE(view->dragMode(), QGraphicsView::ScrollHandDrag);

    TestHelpers::simulateMouseRelease(view->viewport(), Qt::RightButton, pos,
                                      view->viewport()->mapToGlobal(pos));
}

void TestSourceImageView::testRightClickReleasesRestoresDragMode() {
    QPoint pos(100, 100);

    TestHelpers::simulateMousePress(view->viewport(), Qt::RightButton, pos,
                                    view->viewport()->mapToGlobal(pos));
    QCOMPARE(view->dragMode(), QGraphicsView::ScrollHandDrag);

    TestHelpers::simulateMouseRelease(view->viewport(), Qt::RightButton, pos,
                                      view->viewport()->mapToGlobal(pos));

    // Should be back to NoDrag
    QCOMPARE(view->dragMode(), QGraphicsView::NoDrag);
}

void TestSourceImageView::testRightClickDragSequence() {
    QPoint startPos(100, 100);
    QPoint endPos(200, 200);

    // Press
    TestHelpers::simulateMousePress(view->viewport(), Qt::RightButton, startPos,
                                    view->viewport()->mapToGlobal(startPos));
    QCOMPARE(view->dragMode(), QGraphicsView::ScrollHandDrag);

    // Move
    TestHelpers::simulateMouseMove(view->viewport(), endPos,
                                   view->viewport()->mapToGlobal(endPos),
                                   Qt::RightButton);
    QCOMPARE(view->dragMode(), QGraphicsView::ScrollHandDrag);

    // Release
    TestHelpers::simulateMouseRelease(view->viewport(), Qt::RightButton, endPos,
                                      view->viewport()->mapToGlobal(endPos));
    QCOMPARE(view->dragMode(), QGraphicsView::NoDrag);
}

void TestSourceImageView::testWheelZoomIn() {
    QTransform initialTransform = getViewTransform();

    QPoint pos(400, 300);                                  // Center of view
    TestHelpers::simulateWheelEvent(view->viewport(), 120, // Positive = zoom in
                                    pos, view->viewport()->mapToGlobal(pos));

    QTransform newTransform = getViewTransform();

    // After zooming in, scale should be larger
    QVERIFY(newTransform.m11() > initialTransform.m11());
    QVERIFY(newTransform.m22() > initialTransform.m22());
}

void TestSourceImageView::testWheelZoomOut() {
    QTransform initialTransform = getViewTransform();

    QPoint pos(400, 300);
    TestHelpers::simulateWheelEvent(view->viewport(),
                                    -120, // Negative = zoom out
                                    pos, view->viewport()->mapToGlobal(pos));

    QTransform newTransform = getViewTransform();

    // After zooming out, scale should be smaller
    QVERIFY(newTransform.m11() < initialTransform.m11());
    QVERIFY(newTransform.m22() < initialTransform.m22());
}

void TestSourceImageView::testMultipleZoomOperations() {
    QTransform initialTransform = getViewTransform();
    qreal initialScale = initialTransform.m11();

    QPoint pos(400, 300);

    // Zoom in multiple times
    for (int i = 0; i < 3; ++i) {
        TestHelpers::simulateWheelEvent(view->viewport(), 120, pos,
                                        view->viewport()->mapToGlobal(pos));
    }

    QTransform zoomedInTransform = getViewTransform();
    QVERIFY(zoomedInTransform.m11() > initialScale);

    // Zoom out multiple times
    for (int i = 0; i < 6; ++i) {
        TestHelpers::simulateWheelEvent(view->viewport(), -120, pos,
                                        view->viewport()->mapToGlobal(pos));
    }

    QTransform zoomedOutTransform = getViewTransform();
    QVERIFY(zoomedOutTransform.m11() < initialScale);
}

void TestSourceImageView::testZoomChangesTransform() {
    QTransform before = getViewTransform();

    QPoint pos(400, 300);
    TestHelpers::simulateWheelEvent(view->viewport(), 120, pos,
                                    view->viewport()->mapToGlobal(pos));

    QTransform after = getViewTransform();

    // Transform should have changed
    QVERIFY(before != after);
}

void TestSourceImageView::testWithScene() {
    // Scene is already set in init()
    QVERIFY(view->scene() == scene);
    QVERIFY(scene != nullptr);
}

void TestSourceImageView::testWithSceneAndContent() {
    // Add some content to the scene
    QImage testImage = TestHelpers::createTestImage(500, 500);
    QGraphicsPixmapItem* pixmapItem =
        scene->addPixmap(QPixmap::fromImage(testImage));
    QVERIFY(pixmapItem != nullptr);

    // The scene should have items
    QCOMPARE(scene->items().size(), 1);

    // View should be able to see the item
    view->viewport()->update();
    QTest::qWait(50);

    QVERIFY(true); // If we get here without crashing, test passes
}

void TestSourceImageView::testPanningWithContent() {
    // Add content to make panning visible
    QImage testImage = TestHelpers::createTestImage(1000, 1000);
    scene->addPixmap(QPixmap::fromImage(testImage));

    // Set scene rect to ensure scrollbars are available
    scene->setSceneRect(0, 0, 1000, 1000);
    view->setSceneRect(0, 0, 1000, 1000);

    // Center the view at a specific position
    view->centerOn(500, 500);
    QApplication::processEvents();

    // Get initial view center
    QPointF initialCenter = view->mapToScene(view->viewport()->rect().center());

    // Simulate right-click drag with TestHelpers::simulateDrag
    QPoint startPos(400, 300);
    QPoint endPos(500, 400);

    TestHelpers::simulateDrag(view->viewport(), Qt::RightButton, startPos,
                              endPos);

    QPointF newCenter = view->mapToScene(view->viewport()->rect().center());

    // The view center should have moved (panned)
    // Use a larger tolerance to account for test environment variations
    QVERIFY(!TestHelpers::pointFuzzyCompare(initialCenter, newCenter, 5.0));
}

QTEST_MAIN(TestSourceImageView)
#include "test_SourceImageView.moc"
