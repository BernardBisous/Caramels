#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

#include "hardware/Pinout.h"
#include "hardware/analogsensor.h"
#include "hardware/co2manager.h"
#include "hardware/hardwareunit.h"
#include "parameter.h"
#include <QObject>


class DHT{

public:
    AnalogSensor* temp;
    AnalogSensor* humidity;
};

class TemperatureManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit TemperatureManager(QObject *parent = nullptr);
    virtual void reactToParamChanged(Parameter*, float );
    virtual void reactToSensorsChanged();
    virtual QList<Actuator*> interestingIntegrals();

virtual AnalogSensor*regulatingSensor();

    void addDHT(int hpin,int tpin);
    Parameter* temperatureParameter(){return parameterFromId(TEMPERATURE_AIR);}
    Parameter* humidityParameter(){return parameterFromId(HUMIDITY_AIR);}
    Parameter* windParameter(){return parameterFromId(WIND_LEVEL);}

    void regulateHumidity();
    void regulateWind();

    float airTemperature();
    float humidity();

    float humidityExcess();
    float temperatureExcess();
    void setExtractorPower(int purcent);

    void setCo2(CO2Manager *newCo2);

    QList<DHT> dht() const;

private:
    AnalogSensor* m_airSensor;
    AnalogSensor* m_humiditySensor;
    QList<DHT> m_dht;

    SwitchedActuator* m_humidifier;
    SwitchedActuator* m_windpower;
    SwitchedActuator* m_rotation;
    SwitchedActuator* m_extractor;


    CO2Manager* m_co2;

};

#endif // TEMPERATUREMANAGER_H
