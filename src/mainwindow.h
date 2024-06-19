#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include "documentwindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    QMenu* fileMenu;
    QAction* openAction;
    QList<DocumentWindow*>* documents;

  private slots:
    void open();

  signals:
};

#endif // MAINWINDOW_H
