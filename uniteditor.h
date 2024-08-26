#ifndef UNITEDITOR_H
#define UNITEDITOR_H

#include "Interface/devicelistwidget.h"
#include "deviceplot.h"
#include "parameterlistwidget.h"
#include "parameterplot.h"
#include <QWidget>

class UnitEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UnitEditor(QWidget *parent = nullptr);
    void handle(HardwareUnit* h);

signals:
    void editParameter(Parameter* p);
    public slots:
    void editDevice(int i);

private slots:
    void paramSlot(int i,QWidget*);

private:
    HardwareUnit* m_client;
    DeviceListWidget*m_devices;
    DevicePlot*m_devicePlot;
    ScrollArea*m_parameters;
    QWidget* m_parametersWidget;
};

#endif // UNITEDITOR_H
