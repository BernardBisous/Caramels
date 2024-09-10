#include "temperaturemanager.h"


#define FREQ_KEY "Routine freq."

TemperatureManager::TemperatureManager(QObject *parent)
    : HardwareUnit{parent},m_co2(nullptr)
{
    m_name="Température & Humidité";
    attachDevice(m_airSensor=new AnalogSensor(TEMP_1_PIN,"Température de l'air",this));
    attachDevice(m_waterSensor=new AnalogSensor(TEMP_2_PIN,"Température de l'eau",this));
    attachDevice(m_humiditySensor=new AnalogSensor(HUMIDTY_PIN,"Humidité relative",this));
    attachDevice(m_humidifier=new SwitchedActuator(HUMIDIFIER_PIN,false,"Humidificateur"));
    attachDevice(m_windpower=new SwitchedActuator(WIND_POWER_PIN,10,"Ventilateur",this));
    attachDevice(m_rotation=new SwitchedActuator(WIND_ROTATION_PIN,10,"Orienteur du vent",this));
    attachDevice(m_extractor=new SwitchedActuator(EXTRACTOR_PIN,10,"Extracteur",this));

    m_idParameters<<TEMPERATURE_AIR<<HUMIDITY_AIR<<TEMPERATURE_WATER<<WIND_LEVEL<<WIND_ROTATION;


    m_humidifier->setDataValue(FREQ_KEY,QString::number(60));

    m_waterSensor->setUnits("°c");
    m_airSensor->setUnits("°c");
    m_waterSensor->setRange(10,60);
    m_airSensor->setRange(10,60);

    m_humidifier->setGain(100);

}

void TemperatureManager::reactToParamChanged(Parameter *p, float f)
{
    if(p==temperatureParameter())
    {
        m_temperatureCommand=f;
        regulateWind();
    }

    else if(p==humidityParameter())
    {
        m_humidityCommand=f;
        regulateHumidity();

    }


    else if(p==windParameter())
    {
        m_windpower->applyValue(f);
        m_rotation->applyValue(f);
    }
}

void TemperatureManager::reactToSensorsChanged()
{
    if(!activeConfig())
        return;

   if(m_humiditySensor->shouldRegulate())
       regulateHumidity();

   if(m_airSensor->shouldRegulate())
       regulateWind();


}

void TemperatureManager::regulateHumidity()
{
    float hExcess=humidityExcess();
     if(hExcess<0)
     {
         m_humidifier->impulseHigh(-hExcess*m_humidifier->gain());
     }
}

void TemperatureManager::regulateWind()
{
    float co2Excess=0;
    if(m_co2)
        co2Excess=m_co2->excess();

    float hExcess=humidityExcess();
    bool extract=(hExcess>0 || co2Excess>0);

    if(extract)
        m_extractor->userApplyPurcent(100);
    else
        m_extractor->userApplyPurcent(0);
}


float TemperatureManager::airTemperature()
{
    return m_airSensor->currentValue();
}

float TemperatureManager::waterTemperature()
{
     return m_waterSensor->currentValue();
}

float TemperatureManager::humidity()
{
    return m_humiditySensor->currentValue();
}

float TemperatureManager::humidityExcess()
{
    return humidity()-m_humidityCommand;
}

float TemperatureManager::temperatureExcess()
{
    return airTemperature()-m_temperatureCommand;
}

void TemperatureManager::setCo2(CO2Manager *newCo2)
{
    m_co2 = newCo2;
}
