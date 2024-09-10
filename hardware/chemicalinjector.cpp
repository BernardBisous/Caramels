#include "chemicalinjector.h"


#define MIN_INJECTION_MS 100
ChemicalInjector::ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int ID, QObject*parent )
    : QObject(parent),m_mixer(nullptr),
      m_levelSensor(nullptr),m_state(ready),m_id(ID)
{

    m_pump=new Pump(pumpPin,"Pompe engrais ",this);
    connect(m_pump,SIGNAL(impulseDone()),this,SLOT(pumpSlot()));

    if(mixPin>0)
    {
        m_mixer=new SwitchedActuator(mixPin,true,"Brasseur engrais ",this);
        connect(m_mixer,SIGNAL(impulseDone()),this,SLOT(mixerSlot()));

    }




    if(LevelPin>0)
        m_levelSensor=new BooleanSensor(LevelPin,"Niveau engrais ",this);
    else
        m_levelSensor=nullptr;
}


Pump *ChemicalInjector::pump() const
{
    return m_pump;
}



void ChemicalInjector::setPump(Pump *newPump)
{
    m_pump = newPump;
}

void ChemicalInjector::injectMl(float v)
{


    m_injectingValue=v;

    if(m_state==injecting)
    {
        mixerSlot();
        return;
    }

    m_mixer->impulseHigh(m_mixer->gain()*1000);

    setState(mixing);

}


void ChemicalInjector::setGain(float msPerMl)
{
    m_pump->setGain(msPerMl);
}

float ChemicalInjector::gain()
{
    return m_pump->gain();
}

void ChemicalInjector::setName(QString s)
{
    m_pump->setName("Pompe "+ s);

    if(m_mixer)
        m_mixer->setName("Vibreur "+s);

    if(m_levelSensor)
        m_levelSensor->setName("Niveau "+s);
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
    setState(injecting);
    m_pump->inject(m_injectingValue);
}

void ChemicalInjector::pumpSlot()
{

    setState(ready);

}

ChemicalInjector::State ChemicalInjector::state() const
{
    return m_state;
}

void ChemicalInjector::setState(State newState)
{

    emit stateChanged(newState);
    m_state = newState;
}




