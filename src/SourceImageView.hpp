#pragma once

#include <QGraphicsView>
#include <QImage>
#include <QMainWindow>
#include <QWidget>

class SourceImageView : public QGraphicsView {
    Q_OBJECT

  public:
    explicit SourceImageView(QWidget* parent = nullptr);

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

  public slots:

  private slots:

  private:
};
