#include <QtTest/QtTest>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "ExtractedView.h"
#include "test_helpers.h"

class TestExtractedView : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Construction tests
    void testConstruction();
    void testConstructionWithParent();
    void testIsQGraphicsView();

    // Basic functionality tests
    void testSetScene();
    void testSceneWithContent();
    void testResize();
    void testShow();

    // QGraphicsView inherited functionality
    void testTransform();
    void testScale();
    void testViewport();

private:
    ExtractedView* view;
    QGraphicsScene* scene;
};

void TestExtractedView::initTestCase()
{
    view = nullptr;
    scene = nullptr;
}

void TestExtractedView::cleanupTestCase()
{
    // Called after all tests
}

void TestExtractedView::init()
{
    view = new ExtractedView();
    scene = new QGraphicsScene();
}

void TestExtractedView::cleanup()
{
    delete view;
    view = nullptr;
    delete scene;
    scene = nullptr;
}

void TestExtractedView::testConstruction()
{
    QVERIFY(view != nullptr);
}

void TestExtractedView::testConstructionWithParent()
{
    QWidget parent;
    ExtractedView* childView = new ExtractedView(&parent);

    QVERIFY(childView != nullptr);
    QCOMPARE(childView->parentWidget(), &parent);

    // Parent will delete the child
}

void TestExtractedView::testIsQGraphicsView()
{
    // Test that ExtractedView is indeed a QGraphicsView
    QGraphicsView* basePointer = view;
    QVERIFY(basePointer != nullptr);

    // Test dynamic cast
    QGraphicsView* castView = qobject_cast<QGraphicsView*>(view);
    QVERIFY(castView != nullptr);
}

void TestExtractedView::testSetScene()
{
    view->setScene(scene);
    QCOMPARE(view->scene(), scene);
}

void TestExtractedView::testSceneWithContent()
{
    view->setScene(scene);

    // Add content to the scene
    QImage testImage = TestHelpers::createTestImage(300, 300);
    QGraphicsPixmapItem* item = scene->addPixmap(QPixmap::fromImage(testImage));

    QVERIFY(item != nullptr);
    QCOMPARE(scene->items().size(), 1);

    // Verify view can access the scene content
    QVERIFY(view->scene() == scene);
    QCOMPARE(view->scene()->items().size(), 1);
}

void TestExtractedView::testResize()
{
    view->resize(640, 480);

    QCOMPARE(view->width(), 640);
    QCOMPARE(view->height(), 480);
}

void TestExtractedView::testShow()
{
    view->show();
    QTest::qWaitForWindowExposed(view);

    QVERIFY(view->isVisible());
}

void TestExtractedView::testTransform()
{
    QTransform initialTransform = view->transform();

    // Apply a transform
    view->scale(2.0, 2.0);

    QTransform newTransform = view->transform();

    // Transform should have changed
    QVERIFY(newTransform != initialTransform);
    QCOMPARE(newTransform.m11(), 2.0);
    QCOMPARE(newTransform.m22(), 2.0);
}

void TestExtractedView::testScale()
{
    QTransform initial = view->transform();

    view->scale(1.5, 1.5);

    QTransform after = view->transform();

    // Scaling should change the transform
    QVERIFY(after.m11() > initial.m11());
    QVERIFY(after.m22() > initial.m22());
}

void TestExtractedView::testViewport()
{
    QWidget* viewport = view->viewport();

    QVERIFY(viewport != nullptr);
    QVERIFY(viewport->parentWidget() == view);
}

QTEST_MAIN(TestExtractedView)
#include "test_ExtractedView.moc"
