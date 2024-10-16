#ifndef TENTEDITOR_H
#define TENTEDITOR_H

#include <QWidget>

#include "Interface/scrollarea.h"
#include "hardware/tent.h"
#include "widgets/uniteditor.h"


class TentEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TentEditor(QWidget *parent = nullptr);
    void handle(Tent*t);
    void edit(int index);
    void edit(HardwareUnit* s);
    HardwareUnit* currentUnit();

private slots:
    void listSlot(int i, QWidget *);
    void paramListSlot(int i,Parameter* p);
    void showDevices();

private:
    Tent*m_client;
    ScrollArea* m_units;
    int m_currentUnit;
    UnitEditor* m_editor;
    ToolButton* m_devices;



   // SerialEditor* m_ports;
   // ParameterListWidget*m_parameters;
};

#endif // TENTEDITOR_H
