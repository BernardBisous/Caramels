#ifndef SERIALEDITOR_H
#define SERIALEDITOR_H

#include "Interface/menubutton.h"
#include "tent.h"
#include <QWidget>

class SerialEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SerialEditor(QWidget *parent = nullptr);
    void handle(Tent* t);
    void refresh();
signals:

private slots:
    void connectedChanged();
    void portSlot(QString s);

private:

    Tent*m_client;
    MenuButton* m_ports;


};

#endif // SERIALEDITOR_H
