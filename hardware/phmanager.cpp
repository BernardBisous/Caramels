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
    m_phPlus->setGain(100);
    m_phMinus->setGain(100);

    m_idParameters<<PH_LEVEL;
}

void PHManager::reactToParamChanged(Parameter *p, float f)
{

    if(p && p==parameterFromId(PH_LEVEL))
    {
        m_activ=true;
        m_command=f;
    }
}

float PHManager::ph()
{
    return m_sensor->currentValue();
}

void PHManager::reactToSensorsChanged()
{
    if(!m_activ)
    {
        return;
    }
    float err=ph()-m_command;
    if(err>0)
    {
        m_phMinus->injectMl(err);
    }
    else
    {

        m_phPlus->injectMl(-err);
    }
}

void PHManager::finish()
{
    m_activ=false;
    HardwareUnit::finish();
}

void PHManager::attachInjector(ChemicalInjector *c)
{

    attachDevice(c->pump());
    attachDevice(c->mixer());
    attachDevice(c->levelSensor());

    if(c->id()>0)
        m_idParameters<<c->id();

}
