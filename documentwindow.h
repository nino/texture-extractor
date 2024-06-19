#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QMainWindow>

class DocumentWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit DocumentWindow(QMainWindow* parent = nullptr);
    void set_document_title(const QString& new_title);
    QString get_document_title();

  private:
    QString document_title;

  signals:
};

#endif // DOCUMENTWINDOW_H
