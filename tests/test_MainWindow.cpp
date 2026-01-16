#include <QtTest/QtTest>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QSignalSpy>
#include "mainwindow.hpp"
#include "documentwindow.hpp"
#include "test_helpers.hpp"

class TestMainWindow : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Construction tests
    void testConstruction();
    void testMenuBarExists();
    void testFileMenuExists();
    void testOpenActionExists();

    // Menu and action tests
    void testOpenActionProperties();
    void testOpenActionShortcut();
    void testFileMenuContainsOpenAction();

    // Drag and drop tests
    void testAcceptsDrops();
    void testDragEnterWithUrls();
    void testDragEnterWithoutUrls();
    void testDropEventWithLocalFile();
    void testDropEventWithMultipleFiles();

    // File opening tests
    void testOpenFileSingle();
    void testOpenFileCreatesDocumentWindow();
    void testOpenFileMultiple();

  private:
    MainWindow* mainWindow;
    QString testImagePath1;
    QString testImagePath2;
};

void TestMainWindow::initTestCase() {
    // Create test images
    testImagePath1 = TestHelpers::createTempImageFile(300, 300, "PNG");
    testImagePath2 = TestHelpers::createTempImageFile(400, 400, "PNG");

    QVERIFY(!testImagePath1.isEmpty());
    QVERIFY(!testImagePath2.isEmpty());
}

void TestMainWindow::cleanupTestCase() {
    // Clean up test images
    if (!testImagePath1.isEmpty()) {
        QFile::remove(testImagePath1);
    }
    if (!testImagePath2.isEmpty()) {
        QFile::remove(testImagePath2);
    }
}

void TestMainWindow::init() {
    mainWindow = new MainWindow();
}

void TestMainWindow::cleanup() {
    // Close and delete all child DocumentWindows
    QList<DocumentWindow*> docWindows = mainWindow->findChildren<DocumentWindow*>();
    for (DocumentWindow* doc : docWindows) {
        doc->setAttribute(Qt::WA_DeleteOnClose, false);
        doc->close();
        delete doc;
    }

    delete mainWindow;
    mainWindow = nullptr;
}

void TestMainWindow::testConstruction() {
    QVERIFY(mainWindow != nullptr);
}

void TestMainWindow::testMenuBarExists() {
    QMenuBar* menuBar = mainWindow->menuBar();
    QVERIFY(menuBar != nullptr);
}

void TestMainWindow::testFileMenuExists() {
    QMenu* fileMenu = mainWindow->findChild<QMenu*>();
    QVERIFY(fileMenu != nullptr);
}

void TestMainWindow::testOpenActionExists() {
    QAction* openAction = nullptr;

    // Find the open action
    QList<QAction*> actions = mainWindow->findChildren<QAction*>();
    for (QAction* action : actions) {
        if (action->text().contains("Open")) {
            openAction = action;
            break;
        }
    }

    QVERIFY(openAction != nullptr);
}

void TestMainWindow::testOpenActionProperties() {
    QList<QAction*> actions = mainWindow->findChildren<QAction*>();
    QAction* openAction = nullptr;

    for (QAction* action : actions) {
        if (action->text().contains("Open")) {
            openAction = action;
            break;
        }
    }

    QVERIFY(openAction != nullptr);
    QVERIFY(openAction->text().contains("Open"));
    QVERIFY(!openAction->statusTip().isEmpty());
}

void TestMainWindow::testOpenActionShortcut() {
    QList<QAction*> actions = mainWindow->findChildren<QAction*>();
    QAction* openAction = nullptr;

    for (QAction* action : actions) {
        if (action->text().contains("Open")) {
            openAction = action;
            break;
        }
    }

    QVERIFY(openAction != nullptr);

    // Should have the standard Open shortcut (Ctrl+O / Cmd+O)
    QList<QKeySequence> shortcuts = openAction->shortcuts();
    QVERIFY(!shortcuts.isEmpty());

    // The shortcut should match the standard Open shortcut
    bool hasOpenShortcut = false;
    QKeySequence standardOpen = QKeySequence::Open;
    for (const QKeySequence& seq : shortcuts) {
        if (seq == standardOpen) {
            hasOpenShortcut = true;
            break;
        }
    }
    QVERIFY(hasOpenShortcut);
}

void TestMainWindow::testFileMenuContainsOpenAction() {
    QMenu* fileMenu = mainWindow->findChild<QMenu*>();
    QVERIFY(fileMenu != nullptr);

    QList<QAction*> menuActions = fileMenu->actions();
    QVERIFY(!menuActions.isEmpty());

    // Find open action in menu
    bool foundOpen = false;
    for (QAction* action : menuActions) {
        if (action->text().contains("Open")) {
            foundOpen = true;
            break;
        }
    }

    QVERIFY(foundOpen);
}

void TestMainWindow::testAcceptsDrops() {
    QVERIFY(mainWindow->acceptDrops());
}

void TestMainWindow::testDragEnterWithUrls() {
    QMimeData* mimeData = new QMimeData();
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(testImagePath1);
    mimeData->setUrls(urls);

    QDragEnterEvent event(QPoint(100, 100), Qt::CopyAction, mimeData, Qt::LeftButton,
                          Qt::NoModifier);

    // Send the event
    QCoreApplication::sendEvent(mainWindow, &event);

    // Event should be accepted
    QVERIFY(event.isAccepted());

    delete mimeData;
}

void TestMainWindow::testDragEnterWithoutUrls() {
    QMimeData* mimeData = new QMimeData();
    mimeData->setText("Some text");

    QDragEnterEvent event(QPoint(100, 100), Qt::CopyAction, mimeData, Qt::LeftButton,
                          Qt::NoModifier);

    // Send the event
    QCoreApplication::sendEvent(mainWindow, &event);

    // Event should not be accepted (no URLs)
    QVERIFY(!event.isAccepted());

    delete mimeData;
}

void TestMainWindow::testDropEventWithLocalFile() {
    QMimeData* mimeData = new QMimeData();
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(testImagePath1);
    mimeData->setUrls(urls);

    // First send drag enter event
    QDragEnterEvent enterEvent(QPoint(100, 100), Qt::CopyAction, mimeData, Qt::LeftButton,
                               Qt::NoModifier);
    QCoreApplication::sendEvent(mainWindow, &enterEvent);
    QApplication::processEvents();

    // Count DocumentWindows before drop
    int beforeCount = mainWindow->findChildren<DocumentWindow*>().size();

    // Now send the drop event
    QDropEvent event(QPoint(100, 100), Qt::CopyAction, mimeData, Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(mainWindow, &event);

    // Process events to allow window creation
    QApplication::processEvents();
    QTest::qWait(200);
    QApplication::processEvents();

    // Should have created a new DocumentWindow
    int afterCount = mainWindow->findChildren<DocumentWindow*>().size();
    QVERIFY(afterCount > beforeCount);

    delete mimeData;
}

void TestMainWindow::testDropEventWithMultipleFiles() {
    QMimeData* mimeData = new QMimeData();
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(testImagePath1);
    urls << QUrl::fromLocalFile(testImagePath2);
    mimeData->setUrls(urls);

    // First send drag enter event
    QDragEnterEvent enterEvent(QPoint(100, 100), Qt::CopyAction, mimeData, Qt::LeftButton,
                               Qt::NoModifier);
    QCoreApplication::sendEvent(mainWindow, &enterEvent);
    QApplication::processEvents();

    int beforeCount = mainWindow->findChildren<DocumentWindow*>().size();

    // Now send drop event
    QDropEvent event(QPoint(100, 100), Qt::CopyAction, mimeData, Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(mainWindow, &event);

    // Process events to allow window creation
    QApplication::processEvents();
    QTest::qWait(200);
    QApplication::processEvents();

    int afterCount = mainWindow->findChildren<DocumentWindow*>().size();

    // Should have created two new DocumentWindows
    QCOMPARE(afterCount - beforeCount, 2);

    delete mimeData;
}

void TestMainWindow::testOpenFileSingle() {
    int beforeCount = mainWindow->findChildren<DocumentWindow*>().size();

    mainWindow->openFile(testImagePath1);

    int afterCount = mainWindow->findChildren<DocumentWindow*>().size();

    QVERIFY(afterCount > beforeCount);
}

void TestMainWindow::testOpenFileCreatesDocumentWindow() {
    mainWindow->openFile(testImagePath1);

    QList<DocumentWindow*> docWindows = mainWindow->findChildren<DocumentWindow*>();
    QVERIFY(!docWindows.isEmpty());

    // The last created window should have our file path as title
    DocumentWindow* lastDoc = docWindows.last();
    QCOMPARE(lastDoc->getDocumentTitle(), testImagePath1);
}

void TestMainWindow::testOpenFileMultiple() {
    int beforeCount = mainWindow->findChildren<DocumentWindow*>().size();

    mainWindow->openFile(testImagePath1);
    mainWindow->openFile(testImagePath2);

    int afterCount = mainWindow->findChildren<DocumentWindow*>().size();

    // Should have created two new windows
    QCOMPARE(afterCount - beforeCount, 2);
}

QTEST_MAIN(TestMainWindow)
#include "test_MainWindow.moc"
