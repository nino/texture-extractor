#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    QMenu* fileMenu;
    QAction* openAction;

  private slots:
    void open();

  signals:
};

#endif // MAINWINDOW_H
