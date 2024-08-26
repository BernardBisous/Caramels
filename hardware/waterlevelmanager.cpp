#include "waterlevelmanager.h"
#include "hardware/Pinout.h"
#include "hardware/booleansensor.h"
#include "hardware/pump.h"

WaterLevelManager::WaterLevelManager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Arrosage";
    attachDevice(m_pump=new Pump(MAIN_PUMP_PIN,"Pompe d'arrosage"));
    attachDevice(m_entryValve=new SwitchedActuator(MAIN_VALVE_PIN,"Pompe d'arrosage"));
    attachDevice(m_levelUp=new BooleanSensor(WATER_LEVEL_PIN_1,"Cuve pleine"));
    attachDevice(m_levelDown=new BooleanSensor(WATER_LEVEL_PIN_1,"Cuve vide"));



}
