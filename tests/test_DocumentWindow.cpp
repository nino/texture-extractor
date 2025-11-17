#include <QtTest/QtTest>
#include <QMainWindow>
#include <QLabel>
#include <QTemporaryFile>
#include <QFile>
#include "documentwindow.h"
#include "test_helpers.h"

class TestDocumentWindow : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // DocumentWindow tests
    void testDocumentWindowConstruction();
    void testDocumentWindowInitialTitle();
    void testDocumentWindowInitialCentralWidget();
    void testSetDocumentTitle();
    void testGetDocumentTitle();
    void testSetDocumentTitleChangesWindowTitle();
    void testDeleteOnCloseAttribute();
    void testSetDocumentTitleWithValidImage();
    void testSetDocumentTitleMultipleTimes();

    // PhotoView tests
    void testPhotoViewConstruction();
    void testPhotoViewWithValidImage();
    void testPhotoViewWithInvalidImage();
    void testPhotoViewLayout();
    void testPhotoViewHasBothViews();
    void testPhotoViewGraphicsScene();
    void testPhotoViewSceneContent();

private:
    DocumentWindow* docWindow;
    QString testImagePath;
};

void TestDocumentWindow::initTestCase()
{
    // Create a test image file that persists across tests
    testImagePath = TestHelpers::createTempImageFile(400, 300, "PNG");
    QVERIFY(!testImagePath.isEmpty());
}

void TestDocumentWindow::cleanupTestCase()
{
    // Clean up test image
    if (!testImagePath.isEmpty()) {
        QFile::remove(testImagePath);
    }
}

void TestDocumentWindow::init()
{
    docWindow = new DocumentWindow();
}

void TestDocumentWindow::cleanup()
{
    if (docWindow) {
        // Don't use deleteLater since we're in tests
        docWindow->setAttribute(Qt::WA_DeleteOnClose, false);
        delete docWindow;
        docWindow = nullptr;
    }
}

void TestDocumentWindow::testDocumentWindowConstruction()
{
    QVERIFY(docWindow != nullptr);
}

void TestDocumentWindow::testDocumentWindowInitialTitle()
{
    QCOMPARE(docWindow->windowTitle(), QString("New Document"));
}

void TestDocumentWindow::testDocumentWindowInitialCentralWidget()
{
    QWidget* centralWidget = docWindow->centralWidget();
    QVERIFY(centralWidget != nullptr);

    // Should be a QLabel with "Loading..." text
    QLabel* label = qobject_cast<QLabel*>(centralWidget);
    QVERIFY(label != nullptr);
    QCOMPARE(label->text(), QString("Loading..."));
}

void TestDocumentWindow::testSetDocumentTitle()
{
    QString title = "Test Document";
    docWindow->set_document_title(title);

    QCOMPARE(docWindow->get_document_title(), title);
}

void TestDocumentWindow::testGetDocumentTitle()
{
    // Initially should be empty
    QVERIFY(docWindow->get_document_title().isEmpty());

    QString title = "My Document";
    docWindow->set_document_title(title);

    QCOMPARE(docWindow->get_document_title(), title);
}

void TestDocumentWindow::testSetDocumentTitleChangesWindowTitle()
{
    QString title = "Window Title Test";
    docWindow->set_document_title(title);

    QCOMPARE(docWindow->windowTitle(), title);
}

void TestDocumentWindow::testDeleteOnCloseAttribute()
{
    QVERIFY(docWindow->testAttribute(Qt::WA_DeleteOnClose));
}

void TestDocumentWindow::testSetDocumentTitleWithValidImage()
{
    docWindow->set_document_title(testImagePath);

    // The central widget should now be a PhotoView
    QWidget* centralWidget = docWindow->centralWidget();
    QVERIFY(centralWidget != nullptr);

    // Should no longer be the loading label
    QLabel* label = qobject_cast<QLabel*>(centralWidget);
    QVERIFY(label == nullptr);  // Should not be a label anymore
}

void TestDocumentWindow::testSetDocumentTitleMultipleTimes()
{
    docWindow->set_document_title("First Title");
    QCOMPARE(docWindow->get_document_title(), QString("First Title"));

    docWindow->set_document_title("Second Title");
    QCOMPARE(docWindow->get_document_title(), QString("Second Title"));

    docWindow->set_document_title(testImagePath);
    QCOMPARE(docWindow->get_document_title(), testImagePath);
}

void TestDocumentWindow::testPhotoViewConstruction()
{
    PhotoView* view = new PhotoView(testImagePath);
    QVERIFY(view != nullptr);
    delete view;
}

void TestDocumentWindow::testPhotoViewWithValidImage()
{
    PhotoView* view = new PhotoView(testImagePath);
    QVERIFY(view != nullptr);

    // Should not crash with valid image
    view->show();
    QTest::qWait(100);

    delete view;
}

void TestDocumentWindow::testPhotoViewWithInvalidImage()
{
    QString invalidPath = "/nonexistent/path/to/image.png";
    PhotoView* view = new PhotoView(invalidPath);

    // Should not crash even with invalid image
    QVERIFY(view != nullptr);

    delete view;
}

void TestDocumentWindow::testPhotoViewLayout()
{
    PhotoView* view = new PhotoView(testImagePath);

    QLayout* layout = view->layout();
    QVERIFY(layout != nullptr);

    // Should be a QHBoxLayout
    QHBoxLayout* hboxLayout = qobject_cast<QHBoxLayout*>(layout);
    QVERIFY(hboxLayout != nullptr);

    delete view;
}

void TestDocumentWindow::testPhotoViewHasBothViews()
{
    PhotoView* view = new PhotoView(testImagePath);

    // Find child views
    SourceImageView* sourceView = view->findChild<SourceImageView*>();
    ExtractedView* extractedView = view->findChild<ExtractedView*>();

    QVERIFY(sourceView != nullptr);
    QVERIFY(extractedView != nullptr);

    delete view;
}

void TestDocumentWindow::testPhotoViewGraphicsScene()
{
    PhotoView* view = new PhotoView(testImagePath);

    SourceImageView* sourceView = view->findChild<SourceImageView*>();
    QVERIFY(sourceView != nullptr);

    QGraphicsScene* scene = sourceView->scene();
    QVERIFY(scene != nullptr);

    delete view;
}

void TestDocumentWindow::testPhotoViewSceneContent()
{
    PhotoView* view = new PhotoView(testImagePath);

    SourceImageView* sourceView = view->findChild<SourceImageView*>();
    QVERIFY(sourceView != nullptr);

    QGraphicsScene* scene = sourceView->scene();
    QVERIFY(scene != nullptr);

    // Scene should have items (MovableEllipse, EditableRectItem, and image)
    QList<QGraphicsItem*> items = scene->items();
    QVERIFY(items.size() >= 2);  // At least ellipse and rect

    // Should have 3 items: ellipse, editable rect, and pixmap
    QCOMPARE(items.size(), 3);

    delete view;
}

QTEST_MAIN(TestDocumentWindow)
#include "test_DocumentWindow.moc"
