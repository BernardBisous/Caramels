#ifndef CO2MANAGER_H
#define CO2MANAGER_H

#include "hardware/analogsensor.h"
#include "hardware/hardwareunit.h"
#include <QObject>



class CO2Manager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit CO2Manager(QObject *parent = nullptr);
    float CO2();

signals:

private:
    SwitchedActuator*m_injector;
    AnalogSensor* m_sensor;


};

#endif // CO2MANAGER_H
