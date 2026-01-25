#pragma once

#include <QGraphicsView>

class ExtractedView : public QGraphicsView {
   Q_OBJECT

 public:
   explicit ExtractedView(QWidget* parent = nullptr);
};
