#ifndef NAMELABEL_H
#define NAMELABEL_H

#include "qcoreevent.h"
#include "qevent.h"
#include "qlabel.h"
#include "qlineedit.h"
#include <QWidget>


class NameLabel : public QWidget
{
    Q_OBJECT
public:
    explicit NameLabel(QWidget *parent = nullptr);

    void setTextSize(int pointSize);
    void load(QString s);
    void setEditMode(bool s);


    bool eventFilter(QObject* obj, QEvent* event) override {
       if (obj == m_label && event->type() == QEvent::MouseButtonPress) {
         clickedSlot();
         return true; // Stop event propagation
       }
       else if( event->type() == QEvent::FocusOut ) {

           finishedSlot();
       }
       return QObject::eventFilter(obj, event); // Pass other events
     }
    void focusOutEvent(QFocusEvent* event) override {
        if (event->lostFocus()) {
          finishedSlot();
        }
        QWidget::focusOutEvent(event);
      }

public slots:


private slots:
    void clickedSlot();
    void finishedSlot();

signals:
    void edited(QString);
private:

    QLabel* m_label;
    QLineEdit* m_edit;

};

#endif // NAMELABEL_H
