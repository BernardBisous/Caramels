#include "chemicalinjector.h"


ChemicalInjector::ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int ID, QObject*parent )
    : QObject(parent),m_maxMsInjection(2000),m_id(ID)
{

    m_pump=new ChemicalPump(pumpPin,"Pompe engrais ",this);
    m_mixer=new SwitchedActuator(mixPin,true,"Brasseur engrais ",this);

    if(LevelPin>0)
        m_levelSensor=new BooleanSensor(LevelPin,"Niveau engrais ",this);
    else
        m_levelSensor=nullptr;
}


ChemicalPump *ChemicalInjector::pump() const
{
    return m_pump;
}



void ChemicalInjector::setPump(ChemicalPump *newPump)
{
    m_pump = newPump;
}

void ChemicalInjector::injectMl(float v)
{
    if(!m_lastInjection.isValid()
            || (m_lastInjection.msecsTo(QDateTime::currentDateTime())>m_maxMsInjection && m_maxMsInjection>0))
    {

        //m_pump->console("Injecting "+name()+" "+QString::number(v));
        m_lastInjection=QDateTime::currentDateTime();
        m_pump->impulseHigh(msForMl(v));
    }
}

int ChemicalInjector::msForMl(float v)
{
    return m_pump->gain()*v;
}

void ChemicalInjector::setGain(float msPerMl)
{
    m_pump->setGain(msPerMl);
}

float ChemicalInjector::gain()
{
    return m_pump->gain();
}

QDateTime ChemicalInjector::lastInjection() const
{
    return m_lastInjection;
}

void ChemicalInjector::setMaxMsInjection(int newMaxMsInjection)
{
    m_maxMsInjection = newMaxMsInjection;
}

SwitchedActuator *ChemicalInjector::mixer() const
{
    return m_mixer;
}

BooleanSensor *ChemicalInjector::levelSensor() const
{
    return m_levelSensor;
}

int ChemicalInjector::id() const
{
    return m_id;
}

