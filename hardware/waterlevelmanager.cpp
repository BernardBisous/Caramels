#include "waterlevelmanager.h"
#include "hardware/Pinout.h"
#include "hardware/booleansensor.h"
#include "hardware/pump.h"

WaterLevelManager::WaterLevelManager(QObject *parent)
    : HardwareUnit{parent},m_filling(false)
{
    m_name="Arrosage";
    attachDevice(m_pump=new SwitchedActuator(MAIN_PUMP_PIN,false,"Pompe d'arrosage"));
    attachDevice(m_entryValve=new Pump(MAIN_VALVE_PIN,"Pompe d'entrée"));
    attachDevice(m_levelUp=new BooleanSensor(WATER_LEVEL_PIN_1,"Cuve pleine"));
    attachDevice(m_levelDown=new BooleanSensor(WATER_LEVEL_PIN_2,"Cuve vide"));

    m_idParameters<<WET_DELAY<<DRY_DELAY;

    m_entryValve->setRange(0,50);//ml/s
    m_pump->setRange(0,10);
    m_pump->setUnits("mL/s");
    m_pump->setIntegralUnits("mL");
    m_levelDown->setActiveHigh(false);

    setDescription("En charge des niveaux d'eau dans la cuve et dans les racines, suivant les capteurs de niveau dans la cuve.");

    setTimeRegulated(m_pump);
    regulator()->setStateStrings("Immergé","Sec");
}

QList<Device *> WaterLevelManager::interestingDevices()
{
   return QList<Device *>()<<m_pump<<m_entryValve;
}

QList<Actuator *> WaterLevelManager::interestingIntegrals()
{
    return QList<Actuator *> ()<<m_entryValve;
}



void WaterLevelManager::reactToSensorsChanged()
{

    if(!activeConfig())
        return;

    bool h=m_levelUp->currentValue();
    bool l=m_levelDown->currentValue();


    if(!h && !l && !m_filling)
    {
        console("Remplissage de la cuve en cours");
        fillTank();
    }

    else if(h  && !l && m_filling)
    {
        m_entryValve->startInjecting(false);
        console("Remplissage terminé de la cuve " +QString::number(m_entryValve->integral())+"ml");
        m_filling=false;


        emit fillingTank(false);
    }
}

void WaterLevelManager::reactToParamChanged(Parameter *p, float v)
{
    if(!p)
        return;
    if(p==dry())
        regulator()->setDelayLow(v/3600);

    else if (p==wet())
        regulator()->setDelayHigh(v/3600*Parameter::timeMultiplicator());
}



void WaterLevelManager::fillTank()
{
    console("Remplissage eau");
    m_filling=true;
    m_entryValve->startInjecting(true);
    emit fillingTank(true);
}


bool WaterLevelManager::filling() const
{
    return m_filling;
}

bool WaterLevelManager::isDry()
{
     return !regulator()->state();
}



float WaterLevelManager::totalInjected()
{
    return m_entryValve->integral();
}

QList<RealTimeValue> WaterLevelManager::injectedHitstoric()
{
    return m_entryValve->integralHistoric();
}

void WaterLevelManager::switchDry()
{
    regulator()->userSwitch();
}




