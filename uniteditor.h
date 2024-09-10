#ifndef UNITEDITOR_H
#define UNITEDITOR_H

#include "Interface/devicelistwidget.h"
#include "deviceplot.h"
#include "parametereditor.h"
#include "parameterlistwidget.h"
#include "parameterplot.h"
#include "unitplot.h"
#include <QWidget>

class UnitEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UnitEditor(QWidget *parent = nullptr);
    void handle(HardwareUnit* h);

public slots:
    void editDevice(int i);
    void editParameter(Parameter*p);

private slots:




private:
    HardwareUnit* m_client;
    DeviceListWidget*m_devices;
    UnitPlot*m_plot;
    ParameterValueEditor* m_paramEditor;
};

#endif // UNITEDITOR_H
