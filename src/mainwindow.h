#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void openFile(const QString& fileName) noexcept;

  private:
    QMenu* fileMenu;
    QAction* openAction;
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);

  private slots:
    void open() noexcept;

  signals:
};

#endif // MAINWINDOW_H
