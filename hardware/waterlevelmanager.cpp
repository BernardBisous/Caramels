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

    attachInjector(new ChemicalInjector(CHEM_MIX_1_PIN,CHEM_PUMP_1_PIN,NO_PIN,CHEMICAL_1,this));
    attachInjector(new ChemicalInjector(CHEM_MIX_2_PIN,CHEM_PUMP_2_PIN,NO_PIN,CHEMICAL_2,this));
    attachInjector(new ChemicalInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,NO_PIN,CHEMICAL_3,this));
}

void WaterLevelManager::attachInjector(ChemicalInjector *c)
{
    attachDevice(c->pump());
    attachDevice(c->mixer());

    attachDevice(c->levelSensor());
    m_injectors.append(c);

    if(c->id()>0)
        m_idParameters<<c->id();
}
