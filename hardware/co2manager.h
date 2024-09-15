#ifndef CO2MANAGER_H
#define CO2MANAGER_H

#include "hardware/Pinout.h"
#include "hardware/analogsensor.h"
#include "hardware/hardwareunit.h"
#include <QObject>



class CO2Manager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit CO2Manager(QObject *parent = nullptr);
    virtual void reactToParamChanged(Parameter*, float );
    virtual void reactToSensorsChanged();
    float CO2();
    float excess();
    Parameter* parameterCO2(){return parameterFromId(CO2_LEVEL);}

signals:

private:

    SwitchedActuator*m_injector;
    AnalogSensor* m_sensor;
};

#endif // CO2MANAGER_H
