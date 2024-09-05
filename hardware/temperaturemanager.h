#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

#include "hardware/Pinout.h"
#include "hardware/analogsensor.h"
#include "hardware/hardwareunit.h"
#include "parameter.h"
#include <QObject>

class TemperatureManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit TemperatureManager(QObject *parent = nullptr);
    virtual void reactToParamChanged(Parameter*, float );
    Parameter* air(){return parameterFromId(TEMPERATURE_AIR);}
    Parameter* water(){return parameterFromId(TEMPERATURE_WATER);}
    Parameter* humidity(){return parameterFromId(HUMIDITY_AIR);}
    float valueatSensor(int index);
    float humidityValue();

private:
    AnalogSensor* m_airSensor;
    AnalogSensor* m_waterSensor;
    AnalogSensor* m_humiditySensor;
    SwitchedActuator* m_humidifier;

};

#endif // TEMPERATUREMANAGER_H
