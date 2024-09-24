#include "temperaturemanager.h"


TemperatureManager::TemperatureManager(QObject *parent)
    : HardwareUnit{parent},m_co2(nullptr)
{
    m_name="Climat";

    addDHT(HUMIDTY_PIN_1,TEMP_1_PIN);
    addDHT(HUMIDTY_PIN_2,TEMP_2_PIN);
    addDHT(HUMIDTY_PIN_3,TEMP_3_PIN);

    attachDevice(m_humidifier=new SwitchedActuator(HUMIDIFIER_PIN,false,"Humidificateur"));
    attachDevice(m_windpower=new SwitchedActuator(WIND_POWER_PIN,10,"Ventilateur",this));
    attachDevice(m_rotation=new SwitchedActuator(WIND_ROTATION_PIN,10,"Orienteur du vent",this));
    attachDevice(m_extractor=new SwitchedActuator(EXTRACTOR_PIN,10,"Extracteur",this));

    m_humiditySensor=m_dht[1].humidity;
    m_airSensor=m_dht[1].temp;

    m_idParameters<<TEMPERATURE_AIR<<HUMIDITY_AIR<<TEMPERATURE_WATER<<WIND_LEVEL<<WIND_ROTATION;




    m_humidifier->setIntegralUnits("mL");
    m_humidifier->setRange(0,5);//ml/s
    m_humidifier->setUnits("mL/s");

    m_extractor->setUnits("m3/s");
    m_extractor->setIntegralUnits("m3");

    attachCouples(TEMPERATURE_AIR,m_airSensor);
    attachCouples(HUMIDITY_AIR,m_humiditySensor);

    setDescription("Gestion du climat, vents et tempêtes, ainsi que l'extracteur général");
}

void TemperatureManager::reactToParamChanged(Parameter *p, float f)
{
    if(p==temperatureParameter())
    {
        m_airSensor->setCommand(f);
        regulateWind();
    }

    else if(p==humidityParameter())
    {
        m_humiditySensor->setCommand(f);


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

QList<Actuator *> TemperatureManager::interestingIntegrals()
{
    return QList<Actuator *> ()<<m_humidifier;
}

AnalogSensor *TemperatureManager::regulatingSensor()
{
    return m_airSensor;
}

void TemperatureManager::addDHT(int hpin, int tpin)
{
    DHT d;
    d.humidity=new AnalogSensor(hpin,"Température "+QString::number(m_dht.count()+1),this);
    d.temp=new AnalogSensor(tpin,"Humidité "+QString::number(m_dht.count()+1),this);
    attachDevice(d.temp);
    attachDevice(d.humidity);

    d.humidity->setUnits("%");
    d.humidity->setRange(0,100);

    d.temp->setUnits("°c");
    d.temp->setRange(0,100);

    m_dht<<d;
}

void TemperatureManager::regulateHumidity()
{
    float hExcess=humidityExcess();
     if(hExcess<0)
     {
         console("Humidifying "+m_humiditySensor->userValue());
         m_humidifier->impulseHigh(-hExcess*m_humidifier->gain());

     }
     m_humiditySensor->setRegulated();
}

void TemperatureManager::regulateWind()
{
    float co2Excess=0;
    if(m_co2)
        co2Excess=m_co2->excess();

    float hExcess=humidityExcess();
    bool extract=(hExcess>0 || co2Excess>0);


    console("Regulating wind "+m_humiditySensor->userValue()+ " "+m_airSensor->userValue());
    m_extractor->setStateHigh(extract);

    m_airSensor->setRegulated();

}


float TemperatureManager::airTemperature()
{
    return m_airSensor->currentValue();
}



float TemperatureManager::humidity()
{
    return m_humiditySensor->currentValue();
}

float TemperatureManager::humidityExcess()
{
    return m_humiditySensor->errorValue();
}

float TemperatureManager::temperatureExcess()
{
    return m_airSensor->errorValue();
}

void TemperatureManager::setCo2(CO2Manager *newCo2)
{
    m_co2 = newCo2;
}
