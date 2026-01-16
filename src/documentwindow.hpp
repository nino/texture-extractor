#pragma once

#include "ExtractedView.hpp"
#include "SourceImageView.hpp"
#include <QGraphicsView>
#include <QImage>
#include <QMainWindow>
#include <QWidget>

class PhotoView : public QWidget {
    Q_OBJECT

  public:
    explicit PhotoView(QString const& filePath, QWidget* parent = nullptr);

  public slots:

  private slots:

  private:
    SourceImageView* graphics;
    ExtractedView* extractedView;

    QImage image;

    void showImage(QString path);
};

class DocumentWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit DocumentWindow(QMainWindow* parent = nullptr);
    void setDocumentTitle(const QString& newTitle);
    QString getDocumentTitle();

  private:
    QString documentTitle;
    PhotoView* photoView;

  signals:
};
