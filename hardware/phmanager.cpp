#include "phmanager.h"
#include "hardware/Pinout.h"
#include "chemicalinjector.h"


PHManager::PHManager(QObject *parent)
    : HardwareUnit{parent},m_activ(false)
{
    m_name="Gestion du PH";
    attachDevice(m_sensor=new AnalogSensor(PH_SENSOR_PIN,"Capteur de PH",this));
    attachInjector(m_phPlus=new ChemicalInjector(CHEM_PUMP_PH_PLUS_PIN,CHEM_MIX_PH_PLUS_PIN,NO_PIN,-1,this));
    attachInjector(m_phMinus=new ChemicalInjector(CHEM_PUMP_PH_MINUS_PIN,CHEM_MIX_PH_MINUS_PIN,NO_PIN,-1,this));

    m_sensor->setUnits("");
    m_sensor->setRange(0,14);
    m_phPlus->setGain(1); //1s/(PHunit*ml)
    m_phMinus->setGain(1);


    m_idParameters<<PH_LEVEL;

    m_phPlus->setName("PH+");
    m_phMinus->setName("PH-");

    setDescription("En charge de la gestion de l'acidité de l'eau via deux injecteurs dans la cuve");
    attachCouples(PH_LEVEL,m_sensor);
}

void PHManager::reactToParamChanged(Parameter *p, float f)
{

    if(p && p==parameterFromId(PH_LEVEL))
    {
        m_activ=true;
        m_sensor->setCommand(f);
        reactToSensorsChanged();
    }
}

void PHManager::reactToSensorsChanged()
{
    if(m_sensor->shouldRegulate())
    {
        regulate();
    }
}

float PHManager::ph()
{
    return m_sensor->currentValue();
}

QList<ChemicalInjector *> PHManager::injectors()
{
    QList<ChemicalInjector *>  out;
    out<<m_phPlus;
    out<<m_phMinus;
    return out;
}


void PHManager::finish()
{
    m_activ=false;
    HardwareUnit::finish();
}

QList<Actuator *> PHManager::interestingIntegrals()
{
    return QList<Actuator *> ()<<m_phPlus->pump()<<m_phMinus->pump();
}

void PHManager::attachInjector(ChemicalInjector *c)
{

    attachDevice(c->pump());
    attachDevice(c->mixer());
    attachDevice(c->levelSensor());

    c->enable(true);
    if(c->id()>0)
        m_idParameters<<c->id();

}

void PHManager::regulate()
{
    if(!m_activ)
    {
        return;
    }

    float err=m_sensor->errorValue();

    console("Regulating PH "+m_sensor->userValue());

    if(err>0)
    {
        m_phMinus->injectMl(err);
    }
    else
    {
        m_phPlus->injectMl(-err);
    }
    m_sensor->setRegulated();
}
