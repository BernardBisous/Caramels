#include "temperaturemanager.h"

TemperatureManager::TemperatureManager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Température & Humidité";
    attachDevice(m_airSensor=new AnalogSensor(TEMP_1_PIN,"Température de l'air",this));
    attachDevice(m_waterSensor=new AnalogSensor(TEMP_2_PIN,"Température de l'eau",this));
    attachDevice(m_humiditySensor=new AnalogSensor(HUMIDTY_PIN,"Humidité relative",this));
    attachDevice(m_humidifier=new SwitchedActuator(HUMIDIFIER_PIN,false,"Humidificateur"));
    m_idParameters<<TEMPERATURE_AIR<<HUMIDITY_AIR<<TEMPERATURE_WATER;


    m_waterSensor->setUnits("°c");
    m_airSensor->setUnits("°c");
    m_waterSensor->setRange(10,60);
    m_airSensor->setRange(10,60);

}

void TemperatureManager::reactToParamChanged(Parameter *p, float f)
{

}

float TemperatureManager::valueatSensor(int index)
{
    switch (index){
    case 0:return m_waterSensor->currentValue();
    default:return m_airSensor->currentValue();

    }
}

float TemperatureManager::humidityValue()
{
    return m_humiditySensor->currentValue();
}
