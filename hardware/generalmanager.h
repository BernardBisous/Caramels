#ifndef GENERALMANAGER_H
#define GENERALMANAGER_H

#include <QObject>
#include "hardware/hardwareunit.h"

class GeneralManager :public HardwareUnit
{
    Q_OBJECT
public:
    explicit GeneralManager(QObject *parent = nullptr);
    virtual void reactToSensorsChanged();
    virtual QList<Actuator*> interestingIntegrals();
    virtual QList<Device*> interestingDevices();
    virtual void begin();

signals:

private:
    SwitchedActuator* m_relay;
    AnalogSensor* m_sensor;
    AnalogSensor* m_sensor12;
    SwitchedActuator *m_fans;

};

#endif // GENERALMANAGER_H
