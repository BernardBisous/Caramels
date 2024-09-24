#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include "Interface/actionwidget.h"
#include "qlabel.h"
#include "statenotifier.h"


class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);

    void handle(StateNotifier* s);
    void setCurrent(DeviceState* s);

public slots:
    void refresh();
private slots:
    void triggSlot();
    void errorSlot();

signals:
    void trig(DeviceState*s);


private:
    StateNotifier*m_client;
    QLabel* m_nameLabel;
    QLabel* m_diagnose;
    QLabel* m_icon;

   ActionWidget* m_editor;
    DeviceState* m_current;
};



#endif // STATEWIDGET_H
