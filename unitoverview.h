#ifndef UNITOVERVIEW_H
#define UNITOVERVIEW_H

#include "hardware/hardwareunit.h"
#include "qlabel.h"
#include "regulatorwidget.h"
#include "unitplot.h"
#include <QWidget>
#include "parameterplot.h"
#include "integralplot.h"
#include "deviceplot.h"
#include <QStackedWidget>
#include "Interface/toolbutton.h"

class ActuatorWidget:public QWidget
{
    Q_OBJECT
public:
    explicit ActuatorWidget(QWidget *parent = nullptr);
    void handle(Actuator* a);
    void refresh();
    void enablePlot(bool s);
    DevicePlot* m_plot;
    IntegralPlot* m_integralPlot;
    Actuator* m_client;
};



class UnitOverview : public QWidget
{
    Q_OBJECT
public:
    explicit UnitOverview(QWidget *parent = nullptr);
    virtual void handle(HardwareUnit*u);
    virtual void refreshCentral();
    virtual void enableCentral(bool s);

    void resizeEvent(QResizeEvent*e);
    void editParameter(Parameter*p);
    void editDevice(Device*d);
    void showCentral();
    void refresh();

    void printIntegral(QList<Actuator*> d);
    void printParameter(Parameter*p, QDateTime t);
    void printParameters(QList<Parameter*> pl, QDateTime start);
    void printDevice(Device *p);
    void printDevices(QList<Device*> pl);

    void printButtons(QStringList l);


    ParameterPlot *parameter() const;

private slots:
    void buttonTrig();
    void paramTrig();
    void deviceTrig();
    void dateChanged(QDateTime t);
signals:
    void requestParamEdit(Parameter*);
    void requestDeviceEdit(Device*d);

protected:
    HardwareUnit* m_client;
    QLabel* m_name;
    QLabel* m_desc;
    QWidget* m_overviewWidget;
    QWidget* m_buttons;
    ParameterPlot* m_parameter;
    DevicePlot* m_device;
    QStackedWidget* m_stack;

    QList<ParameterPlot*> m_parameters;
    QList<DevicePlot*> m_devices;

   QList<ToolButton*>m_buttonList;

    RegulatorWidget* m_regulator;

    QWidget* m_parametersWidget;
    QWidget* m_devicesWidget;

    IntegralPlot* m_integral;





};

#endif // UNITOVERVIEW_H
