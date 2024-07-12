#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include "ExtractedView.h"
#include "SourceImageView.h"
#include <QGraphicsView>
#include <QImage>
#include <QMainWindow>
#include <QWidget>

class PhotoView : public QWidget {
    Q_OBJECT

  public:
    explicit PhotoView(QString const& file_path, QWidget* parent = nullptr);

  public slots:

  private slots:

  private:
    SourceImageView* graphics;
    ExtractedView* extracted_view;

    QImage image;

    void show_image(QString path);
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
