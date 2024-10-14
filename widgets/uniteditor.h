#ifndef UNITEDITOR_H
#define UNITEDITOR_H

#include "Interface/devicelistwidget.h"
#include "deviceplot.h"
#include "parametereditor.h"
#include "parameterlistwidget.h"
#include "parameterplot.h"
#include "unitoverview.h"
#include "unitplot.h"
#include <QWidget>

class UnitEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UnitEditor(QWidget *parent = nullptr);
    void handle(HardwareUnit* h);
    void showDevices(bool s);
    void showDevices();
    static UnitOverview* createOverview(HardwareUnit*u);

    ParameterValueEditor *paramEditor() const;

    UnitOverview *overview() const;

signals:
    void editDeviceRequest(Device*d);
    void editParameterRequest(Parameter*p);


public slots:
    void editDevice(int i);
    void editParameter(Parameter*p);
    void editDevice(Device*d);

private slots:

private:
    HardwareUnit* m_client;
    DeviceListWidget*m_devices;
    UnitOverview* m_overview;
    ParameterValueEditor* m_paramEditor;
    QStackedWidget* m_stack;

};

#endif // UNITEDITOR_H
