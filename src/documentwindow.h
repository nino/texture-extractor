#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QGraphicsView>
#include <QImage>
#include <QMainWindow>
#include <QWidget>

class PhotoView : public QWidget {
    Q_OBJECT

  public:
    explicit PhotoView(QWidget* parent = nullptr);

  public slots:
    void show_image(QString path);

  private slots:
    void zoom_in();
    void zoom_out();

  private:
    QGraphicsView* graphics;
};

class DocumentWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit DocumentWindow(QMainWindow* parent = nullptr);
    void set_document_title(const QString& new_title);
    QString get_document_title();

  private:
    QString document_title;
    PhotoView* photo_view;

  signals:
};

#endif // DOCUMENTWINDOW_H
