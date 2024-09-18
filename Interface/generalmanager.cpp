#include "generalmanager.h"
#include "hardware/analogsensor.h"
#include "hardware/Pinout.h"
#define VOLTAGE 220
#define SENSOR_MAX 10//A
GeneralManager::GeneralManager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Machine";
    setDescription("Gestion de l'alimentation générale, le switch désactive seulement les périphériques");

    attachDevice(m_relay=new SwitchedActuator(GENERAL_PIN,false,"Alimentation générale",this));
    attachDevice(m_sensor=new AnalogSensor(POWER_SENSE,"Courant consomé")),this;
    attachDevice(m_fans=new SwitchedActuator(INTERNAL_FAN_PIN,false,"Ventilation interne",this));


    m_relay->setUnits("kW");
    m_sensor->setUnits("A");
    m_sensor->setRange(0,SENSOR_MAX);

    m_relay->setIntegralUnits("kWs");


    setTimeRegulated(m_relay);
}

void GeneralManager::reactToSensorsChanged()
{
    float v=m_sensor->currentValue();
    m_relay->setMaxRange(v);
}

QList<Actuator *> GeneralManager::interestingIntegrals()
{
    return QList<Actuator *> ()<<m_relay;
}

QList<Device *> GeneralManager::interestingDevices()
{
    return m_devices;
}

void GeneralManager::begin()
{
    HardwareUnit::begin();
    m_relay->setStateHigh(true);
}


