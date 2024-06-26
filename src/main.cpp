#include "mainwindow.h"

#include <QApplication>
#include <QFileOpenEvent>
#include <QLocale>
#include <QTranslator>

class MyApplication : public QApplication {
  public:
    MyApplication(int& argc, char** argv) : QApplication(argc, argv) {}

    bool event(QEvent* event) override {
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent* openEvent = static_cast<QFileOpenEvent*>(event);
            QString file = openEvent->file();
            if (m_mainWindow) {
                m_mainWindow->openFile(file);
            }
            return true;
        }
        return QApplication::event(event);
    }

    void setMainWindow(MainWindow* mainWindow) { m_mainWindow = mainWindow; }

  private:
    MainWindow* m_mainWindow = nullptr;
};

int main(int argc, char* argv[]) {
    auto a = MyApplication(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages) {
        const QString baseName = "texture_extractor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    a.setMainWindow(&w);
    w.show();
    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
