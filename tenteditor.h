#ifndef TENTEDITOR_H
#define TENTEDITOR_H

#include "Interface/devicelistwidget.h"
#include "Interface/menubutton.h"
#include "Interface/scrollarea.h"
#include "Interface/serialeditor.h"
#include "deviceeditor.h"
#include "deviceplot.h"
#include "parameterlistwidget.h"
#include "tent.h"
#include "uniteditor.h"
#include <QWidget>

class TentEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TentEditor(QWidget *parent = nullptr);
    void handle(Tent*t);
    void edit(int index);
    void edit(HardwareUnit* s);
    HardwareUnit* currentUnit();


signals:
    void editParam(Parameter*p);



private slots:
    void listSlot(int i, QWidget *);
    void paramListSlot(Parameter*p);




private:
    Tent*m_client;
    ScrollArea* m_units;
    int m_currentUnit;
    UnitEditor* m_editor;
    SerialEditor* m_ports;


};

#endif // TENTEDITOR_H
