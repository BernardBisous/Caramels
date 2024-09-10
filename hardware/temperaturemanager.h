#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

#include "hardware/Pinout.h"
#include "hardware/analogsensor.h"
#include "hardware/co2manager.h"
#include "hardware/hardwareunit.h"
#include "parameter.h"
#include <QObject>

class TemperatureManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit TemperatureManager(QObject *parent = nullptr);
    virtual void reactToParamChanged(Parameter*, float );
    virtual void reactToSensorsChanged();


    Parameter* temperatureParameter(){return parameterFromId(TEMPERATURE_AIR);}
    Parameter* humidityParameter(){return parameterFromId(HUMIDITY_AIR);}
    Parameter* windParameter(){return parameterFromId(WIND_LEVEL);}

    void regulateHumidity();
    void regulateWind();

    float airTemperature();
    float waterTemperature();
    float humidity();

    float humidityExcess();
    float temperatureExcess();
    void setExtractorPower(int purcent);

    void setCo2(CO2Manager *newCo2);

private:
    AnalogSensor* m_airSensor;
    AnalogSensor* m_waterSensor;
    AnalogSensor* m_humiditySensor;
    SwitchedActuator* m_humidifier;
    SwitchedActuator* m_windpower;
    SwitchedActuator* m_rotation;
    SwitchedActuator* m_extractor;
    float m_humidityCommand;
    float m_temperatureCommand;


    CO2Manager* m_co2;

};

#endif // TEMPERATUREMANAGER_H
