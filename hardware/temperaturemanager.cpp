#include "temperaturemanager.h"

TemperatureManager::TemperatureManager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Température & Humidité";
    attachDevice(m_airSensor=new AnalogSensor(TEMP_1_PIN,10,"Température de l'air",this));
    attachDevice(m_waterSensor=new AnalogSensor(TEMP_2_PIN,10,"Température de l'eau",this));
    attachDevice(m_humiditySensor=new AnalogSensor(HUMIDTY_PIN,10,"Humidité relative",this));
    attachDevice(m_humidifier=new SwitchedActuator(HUMIDIFIER_PIN,false,"Humidificateur"));
    m_idParameters<<TEMPERATURE_AIR<<HUMIDITY_AIR<<TEMPERATURE_WATER;
}

void TemperatureManager::reactToParamChanged(Parameter *p, float f)
{

}
