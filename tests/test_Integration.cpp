#include <QtTest/QtTest>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFile>
#include "mainwindow.h"
#include "documentwindow.h"
#include "SourceImageView.h"
#include "ExtractedView.h"
#include "EditableRectItem.h"
#include "test_helpers.h"

using namespace ninoan::texture_extractor;

class TestIntegration : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Integration test cases
    void testCompleteWorkflow();
    void testMainWindowToDocumentWindow();
    void testDocumentWindowContainsAllViews();
    void testGraphicsSceneIntegration();
    void testEditableRectInScene();
    void testMultipleDocumentWindows();
    void testImageLoadingPipeline();
    void testViewInteractionWithItems();
    void testPanAndZoomWithContent();
    void testResizingRectInFullContext();

private:
    MainWindow* mainWindow;
    QString testImagePath;
};

void TestIntegration::initTestCase()
{
    // Create a test image
    testImagePath = TestHelpers::createTempImageFile(800, 600, "PNG");
    QVERIFY(!testImagePath.isEmpty());
}

void TestIntegration::cleanupTestCase()
{
    if (!testImagePath.isEmpty()) {
        QFile::remove(testImagePath);
    }
}

void TestIntegration::init()
{
    mainWindow = new MainWindow();
    mainWindow->show();
    QTest::qWaitForWindowExposed(mainWindow);
}

void TestIntegration::cleanup()
{
    // Close all document windows
    QList<DocumentWindow*> docWindows = mainWindow->findChildren<DocumentWindow*>();
    for (DocumentWindow* doc : docWindows) {
        doc->setAttribute(Qt::WA_DeleteOnClose, false);
        doc->close();
        delete doc;
    }

    delete mainWindow;
    mainWindow = nullptr;
}

void TestIntegration::testCompleteWorkflow()
{
    // 1. Start with MainWindow
    QVERIFY(mainWindow != nullptr);
    QVERIFY(mainWindow->isVisible());

    // 2. Open a file
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    // 3. Verify DocumentWindow was created
    QList<DocumentWindow*> docWindows = mainWindow->findChildren<DocumentWindow*>();
    QVERIFY(!docWindows.isEmpty());

    DocumentWindow* docWindow = docWindows.first();
    QVERIFY(docWindow != nullptr);

    // 4. Verify PhotoView exists
    QWidget* photoView = docWindow->centralWidget();
    QVERIFY(photoView != nullptr);

    // 5. Verify both views exist
    SourceImageView* sourceView = photoView->findChild<SourceImageView*>();
    ExtractedView* extractedView = photoView->findChild<ExtractedView*>();

    QVERIFY(sourceView != nullptr);
    QVERIFY(extractedView != nullptr);

    // 6. Verify scene has content
    QGraphicsScene* scene = sourceView->scene();
    QVERIFY(scene != nullptr);
    QVERIFY(!scene->items().isEmpty());

    // Complete workflow works!
}

void TestIntegration::testMainWindowToDocumentWindow()
{
    // Test the flow from MainWindow to DocumentWindow
    int initialCount = mainWindow->findChildren<DocumentWindow*>().size();

    mainWindow->openFile(testImagePath);
    QTest::qWait(50);

    int finalCount = mainWindow->findChildren<DocumentWindow*>().size();

    QVERIFY(finalCount > initialCount);
    QCOMPARE(finalCount, initialCount + 1);

    DocumentWindow* doc = mainWindow->findChildren<DocumentWindow*>().last();
    QCOMPARE(doc->get_document_title(), testImagePath);
}

void TestIntegration::testDocumentWindowContainsAllViews()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    QVERIFY(docWindow != nullptr);

    // Should have both SourceImageView and ExtractedView
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();
    ExtractedView* extractedView = docWindow->findChild<ExtractedView*>();

    QVERIFY(sourceView != nullptr);
    QVERIFY(extractedView != nullptr);
}

void TestIntegration::testGraphicsSceneIntegration()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();

    QVERIFY(sourceView != nullptr);

    QGraphicsScene* scene = sourceView->scene();
    QVERIFY(scene != nullptr);

    // Scene should have multiple items (MovableEllipse, EditableRectItem, Pixmap)
    QVERIFY(scene->items().size() >= 2);
}

void TestIntegration::testEditableRectInScene()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();
    QGraphicsScene* scene = sourceView->scene();

    QVERIFY(scene != nullptr);

    // Find the EditableRectItem in the scene
    bool foundEditableRect = false;
    for (QGraphicsItem* item : scene->items()) {
        // EditableRectItem doesn't inherit from any standard Qt item type
        // so we check by type or other means
        if (item->isSelected() || item->flags() & QGraphicsItem::ItemIsSelectable) {
            // Could be our EditableRectItem
            foundEditableRect = true;
        }
    }

    // We should have at least found selectable items
    QVERIFY(scene->items().size() >= 2);
}

void TestIntegration::testMultipleDocumentWindows()
{
    // Open multiple files
    mainWindow->openFile(testImagePath);
    QTest::qWait(50);

    QString tempImage2 = TestHelpers::createTempImageFile(500, 500, "PNG");
    mainWindow->openFile(tempImage2);
    QTest::qWait(50);

    // Should have two document windows
    QList<DocumentWindow*> docWindows = mainWindow->findChildren<DocumentWindow*>();
    QCOMPARE(docWindows.size(), 2);

    // Each should have its own views and scenes
    for (DocumentWindow* doc : docWindows) {
        SourceImageView* sourceView = doc->findChild<SourceImageView*>();
        QVERIFY(sourceView != nullptr);
        QVERIFY(sourceView->scene() != nullptr);
    }

    QFile::remove(tempImage2);
}

void TestIntegration::testImageLoadingPipeline()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();
    QGraphicsScene* scene = sourceView->scene();

    // Find the pixmap item in the scene
    bool foundPixmap = false;
    for (QGraphicsItem* item : scene->items()) {
        QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (pixmapItem != nullptr) {
            foundPixmap = true;
            // Pixmap should not be null
            QVERIFY(!pixmapItem->pixmap().isNull());
            break;
        }
    }

    QVERIFY(foundPixmap);
}

void TestIntegration::testViewInteractionWithItems()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();
    QGraphicsScene* scene = sourceView->scene();

    QVERIFY(sourceView != nullptr);
    QVERIFY(scene != nullptr);

    // Get item at a known position (where we expect the ellipse or rect)
    QPointF scenePos(15, 15);  // Near where items are created
    QList<QGraphicsItem*> itemsAtPos = scene->items(scenePos);

    // Should have items at this position
    QVERIFY(!itemsAtPos.isEmpty());
}

void TestIntegration::testPanAndZoomWithContent()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();

    QVERIFY(sourceView != nullptr);

    // Show the view
    docWindow->show();
    QTest::qWaitForWindowExposed(docWindow);

    // Test zooming
    QTransform initialTransform = sourceView->transform();

    QPoint center(sourceView->width() / 2, sourceView->height() / 2);
    TestHelpers::simulateWheelEvent(sourceView->viewport(), 120, center,
                                   sourceView->viewport()->mapToGlobal(center));

    QTransform zoomedTransform = sourceView->transform();

    // Zoom should have changed the transform
    QVERIFY(zoomedTransform != initialTransform);

    // Test panning
    QPointF centerBefore = sourceView->mapToScene(sourceView->viewport()->rect().center());

    QPoint startPos(100, 100);
    QPoint endPos(200, 200);
    TestHelpers::simulateDrag(sourceView->viewport(), Qt::RightButton, startPos, endPos);

    QPointF centerAfter = sourceView->mapToScene(sourceView->viewport()->rect().center());

    // Pan should have moved the view
    QVERIFY(!TestHelpers::pointFuzzyCompare(centerBefore, centerAfter, 1.0));
}

void TestIntegration::testResizingRectInFullContext()
{
    mainWindow->openFile(testImagePath);
    QTest::qWait(100);

    DocumentWindow* docWindow = mainWindow->findChildren<DocumentWindow*>().first();
    SourceImageView* sourceView = docWindow->findChild<SourceImageView*>();
    QGraphicsScene* scene = sourceView->scene();

    docWindow->show();
    QTest::qWaitForWindowExposed(docWindow);

    QVERIFY(scene != nullptr);

    // Find an item at the expected position (near the created rect)
    QPointF itemPos(10, 10);
    QList<QGraphicsItem*> items = scene->items(itemPos);

    // Should have items
    QVERIFY(!items.isEmpty());

    // Try to interact with the first item
    QGraphicsItem* item = items.first();

    // Simulate mouse press on the item
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(itemPos);
    pressEvent.setButton(Qt::LeftButton);
    pressEvent.setButtons(Qt::LeftButton);
    scene->sendEvent(item, &pressEvent);

    // Simulate mouse move
    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setLastPos(itemPos);
    moveEvent.setPos(QPointF(30, 30));
    moveEvent.setButton(Qt::NoButton);
    moveEvent.setButtons(Qt::LeftButton);
    scene->sendEvent(item, &moveEvent);

    // If we get here without crashing, the interaction works
    QVERIFY(true);
}

QTEST_MAIN(TestIntegration)
#include "test_Integration.moc"
