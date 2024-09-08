#include "co2manager.h"
#include "hardware/Pinout.h"

#define FREQ_KEY "Routine freq."
CO2Manager::CO2Manager(QObject *parent)
    : HardwareUnit{parent},m_lastReg()
{
    m_name="Gestion du CO2";
    attachDevice(m_injector=new SwitchedActuator(CO2_INJECTOR_PIN,false,"Injecteur CO2",this));
    m_injector->setDataValue(FREQ_KEY,QString::number(10));
    attachDevice(m_sensor=new AnalogSensor(CO2_SENSOR_PIN,"Capteur de CO2",this));

    m_idParameters<<CO2_LEVEL;
}

void CO2Manager::reactToParamChanged(Parameter *, float v)
{


    m_command=v;
    regulate();
}

void CO2Manager::reactToSensorsChanged()
{
    if(!activeConfig())
        return;
    int secs=-1;

    if(m_lastReg.isValid())
        secs=m_lastReg.secsTo(QDateTime::currentDateTime());

    if(secs>0 && secs<routineSecs())
        regulate();
}

float CO2Manager::CO2()
{
    return m_sensor->currentValue();
}

float CO2Manager::excess()
{
    return CO2()-m_command;
}

void CO2Manager::regulate()
{
    float err=excess();
    if(err<0)
    {
        m_injector->impulseHigh(err*m_injector->gain());
    }
    m_lastReg=QDateTime::currentDateTime();
}

int CO2Manager::routineSecs()
{
    return m_injector->dataValue(FREQ_KEY).toInt();
}
