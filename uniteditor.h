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

signals:

private:
    DeviceListWidget*m_devices;
    DevicePlot*m_devicePlot;
    ParameterPlot* m_paramPlot;
    ParameterListWidget*m_parameters;
};

#endif // UNITEDITOR_H
