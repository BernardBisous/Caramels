#include "chemicalinjector.h"

#define DELAY_MIX 1000

ChemicalInjector::ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int ID, QObject*parent )
    : QObject(parent),m_maxMsInjection(2000),m_id(ID),m_injecting(false)
{

    m_pump=new ChemicalPump(pumpPin,"Pompe engrais ",this);
    m_mixer=new SwitchedActuator(mixPin,true,"Brasseur engrais ",this);

    connect(m_mixer,SIGNAL(impulseDone()),this,SLOT(mixerSlot()));
    connect(m_pump,SIGNAL(impulseDone()),this,SLOT(pumpSlot()));

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

        m_injectingValue=v;
        m_lastInjection=QDateTime::currentDateTime();
        m_mixer->impulseHigh(DELAY_MIX);
        emit injection(DELAY_MIX + msForMl(v));
        emit console(QString::number(m_id)+" Inject "+QString::number(v)+"ml, "+QString::number(msForMl(v)));


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

void ChemicalInjector::mixerSlot()
{


    m_pump->impulseHigh(msForMl(m_injectingValue));
}

void ChemicalInjector::pumpSlot()
{
    emit injection(0);
}

bool ChemicalInjector::injecting() const
{
    return m_injecting;
}


