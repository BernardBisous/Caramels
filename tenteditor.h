#ifndef TENTEDITOR_H
#define TENTEDITOR_H

#include "Interface/scrollarea.h"
#include "deviceeditor.h"
#include "deviceplot.h"
#include "tent.h"
#include <QWidget>

class TentEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TentEditor(QWidget *parent = nullptr);
    void handle(Tent*t);
    void edit(int index);
    void edit(Device* s);

signals:

private slots:
    void listSlot(int, QWidget*);

private:
    Tent*m_client;
    ScrollArea*m_devices;
    DevicePlot* m_plot;
    DeviceEditor* m_editor;

};

#endif // TENTEDITOR_H
