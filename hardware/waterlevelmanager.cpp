#include "waterlevelmanager.h"
#include "hardware/Pinout.h"
#include "hardware/booleansensor.h"
#include "hardware/pump.h"

WaterLevelManager::WaterLevelManager(QObject *parent)
    : HardwareUnit{parent},m_injectingState("")
{
    m_name="Arrosage";
    attachDevice(m_pump=new Pump(MAIN_PUMP_PIN,"Pompe d'arrosage"));
    attachDevice(m_entryValve=new SwitchedActuator(MAIN_VALVE_PIN,"Pompe d'arrosage"));
    attachDevice(m_levelUp=new BooleanSensor(WATER_LEVEL_PIN_1,"Cuve pleine"));
    attachDevice(m_levelDown=new BooleanSensor(WATER_LEVEL_PIN_2,"Cuve vide"));

    attachInjector(new TankInjector(CHEM_MIX_1_PIN,CHEM_PUMP_1_PIN,NO_PIN,CHEMICAL_1,this));
    attachInjector(new TankInjector(CHEM_MIX_2_PIN,CHEM_PUMP_2_PIN,NO_PIN,CHEMICAL_2,this));
    attachInjector(new TankInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,NO_PIN,CHEMICAL_3,this));

    m_idParameters<<CHEMICAL_1<<CHEMICAL_2<<CHEMICAL_3;
}

void WaterLevelManager::attachInjector(TankInjector *c)
{
    attachDevice(c->pump());
    attachDevice(c->mixer());

    attachDevice(c->levelSensor());
    m_injectors.append(c);
    connect(c,SIGNAL(injection(int)),this,SLOT(injectorSlot(int)));
    connect(c,SIGNAL(console(QString)),this,SLOT(injectorConsole(QString)));

    if(c->id()>0)
        m_idParameters<<c->id();
}

void WaterLevelManager::reactToSensorsChanged()
{

    if(!activeConfig() || injecting())
        return;

    bool h=m_levelUp->currentValue();
    bool l=m_levelDown->currentValue();

    if(!l && !h)
    {
        fillTank();
    }

    else if(h&&l&&injecting())
    {
        console("Remplissage terminé");
        m_injectingState="";
        emit injectingStateChanged(m_injectingState);
    }
}

void WaterLevelManager::reactToParamChanged(Parameter *p, float v)
{
    for(int i=0;i<m_injectors.count();i++)
        if(m_injectors[i]->id()==p->id())
        {
            m_injectors[i]->setCurrentValue(v);
        }
}

void WaterLevelManager::attachParameter(Parameter *p)
{
    HardwareUnit::attachParameter(p);

    if(!p)
        return;

    for(int i=0;i<m_injectors.count();i++)
    {
        if(m_injectors[i]->id()==p->id())
        {
            m_injectors[i]->setName(p->name());
            return;
        }
    }
}

void WaterLevelManager::fillTank()
{

    for(int i=0;i<m_injectors.count();i++)
    {
       m_injectors[i]->fillTank();
    }
    console("Remplissage (injection)");
    m_injectingState="Dosage";

    emit injectingStateChanged(m_injectingState);



}



void WaterLevelManager::injectorSlot(int index)
{
    bool b=true;
    for(int i=0;i<m_injectors.count();i++)
    {
       if(m_injectors[i]->state()!=ChemicalInjector::ready)
           b=false;
    }

    if(b)
    {
        console("Mixage terminé");
        m_injectingState="Remplissage";
        emit injectingStateChanged(m_injectingState);
        m_entryValve->userApplyPurcent(100);
    }

}



QString WaterLevelManager::injectingState() const
{
    return m_injectingState;
}

bool WaterLevelManager::injecting()
{
    return !m_injectingState.isEmpty();
}

TankInjector::TankInjector(int mixPin, int pumpPin, int LevelPin ,int ID, QObject*parent):
    ChemicalInjector(mixPin,pumpPin,LevelPin,ID,parent), m_current(0)
{
    setGain(120);
}



void TankInjector::setCurrentValue(float v)
{
    m_current=v;

}

void TankInjector::fillTank()
{

    injectMl(m_current);
}

