#ifndef EXTRACTEDVIEW_H
#define EXTRACTEDVIEW_H

#include <QGraphicsView>

class ExtractedView : public QGraphicsView {
    Q_OBJECT

  public:
    explicit ExtractedView(QWidget* parent = nullptr);
};

#endif // EXTRACTEDVIEW_H
