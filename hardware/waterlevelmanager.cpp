#include "waterlevelmanager.h"
#include "hardware/Pinout.h"
#include "hardware/booleansensor.h"
#include "hardware/pump.h"

WaterLevelManager::WaterLevelManager(QObject *parent)
    : HardwareUnit{parent},m_dry(0),m_wet(0),m_filling(false),m_currentlyDry(true)
{
    m_name="Arrosage";
    attachDevice(m_pump=new SwitchedActuator(MAIN_PUMP_PIN,false,"Pompe d'arrosage"));
    attachDevice(m_entryValve=new Pump(MAIN_VALVE_PIN,"Pompe d'entrée"));
    attachDevice(m_levelUp=new BooleanSensor(WATER_LEVEL_PIN_1,"Cuve pleine"));
    attachDevice(m_levelDown=new BooleanSensor(WATER_LEVEL_PIN_2,"Cuve vide"));

    m_idParameters<<WET_DELAY<<DRY_DELAY;

    m_dryTimer=new QTimer(this);
    connect(m_dryTimer,SIGNAL(timeout()),this,SLOT(switchDry()));

    m_entryValve->setRange(0,50);//ml/s
    m_pump->setRange(0,10);
    m_pump->setUnits("mL/s");
    m_pump->setIntegralUnits("mL");
    m_levelDown->setActiveHigh(false);

    setDescription("En charge des niveaux d'eau dans la cuve et dans les racines, suivant les capteurs de niveau dans la cuve.");
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

    if(!h && !m_filling)
    {
        console("Remplissage de la cuve en cours");
        fillTank();
    }

    else if(h  && m_filling)
    {
        m_entryValve->startInjecting(false);
        console("Remplissage terminé de la cuve " +QString::number(m_entryValve->integral())+"ml");
        m_filling=false;


        emit fillingTank(false);
    }
}

void WaterLevelManager::reactToParamChanged(Parameter *p, float v)
{

   // qDebug()<<"level params"<<v<<p->name();
    if(!p)
        return;
    if(p==dry())
        m_dry=v;
    else if (p==wet())
        m_wet=v;

    if(!m_dryTimer->isActive() && activeConfig())
    {
        setDry(false);
    }
}

void WaterLevelManager::finish()
{
     m_dryTimer->stop();
     HardwareUnit::finish();
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
     return m_currentlyDry;
}

void WaterLevelManager::setDry(bool s)
{
    if(!s)
    {
        m_pump->userApplyPurcent(100);
    }

    else
    {
        m_pump->userApplyPurcent(0);
    }

    m_currentlyDry=s;

    if(activeConfig())
    {

        if(s)
        {
            console("sechage des racines pour "+QString::number(m_dry)+" secondes");
            m_dryTimer->start(m_dry*1000);
        }
        else
        {
            console("Mouillage des racines pour "+QString::number(m_wet)+" heures");

            m_dryTimer->start(m_wet*1000*Parameter::timeMultiplicator());
        }
    }

    else
        m_dryTimer->stop();
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

    setDry(!isDry());
}




