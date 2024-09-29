#include "chemicalinjector.h"

#define VOLUME_ML 1000
#define MIN_INJECTION_MS 1000
#define MIXER_POWER_W 40
#define PUMP_FLOW 10 //mL/s
ChemicalInjector::ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int ID, QObject*parent )
    : QObject(parent),m_mixer(nullptr),m_levelSensor(nullptr),
      m_state(ready),m_id(ID),m_enable(false),m_volumeMl(VOLUME_ML)
{

    m_pump=new Pump(pumpPin,"Pompe engrais ",this);
    m_pump->setFlow(PUMP_FLOW);
    connect(m_pump,SIGNAL(impulseDone()),this,SLOT(pumpSlot()));

    if(mixPin>0)
    {
        m_mixer=new SwitchedActuator(mixPin,true,"Brasseur engrais ",this);
        connect(m_mixer,SIGNAL(impulseDone()),this,SLOT(mixerSlot()));

        m_mixer->setUnits("W");
        m_mixer->setIntegralUnits("Ws");
        m_mixer->setRange(0,MIXER_POWER_W);
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



    if(m_mixer)
    {
        m_mixer->impulseHigh(m_mixer->gain()*1000);
        setState(mixing);
    }
    else
    {

        m_pump->inject(m_injectingValue);
        setState(injecting);
    }




}


void ChemicalInjector::setGain(float msPerMl)
{
    m_pump->setRange(0,msPerMl);
}

float ChemicalInjector::gain()
{
    return m_pump->gain();
}

bool ChemicalInjector::isLow()
{
    if(m_levelSensor)
        return m_levelSensor->currentValue();

    return false;
}

void ChemicalInjector::setName(QString s)
{
    m_name=s;
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

void ChemicalInjector::enable(bool s)
{
    m_pump->enable(s);

    if(m_mixer)
        m_mixer->enable(s);

    if(m_levelSensor)
        m_levelSensor->enable(s);

    m_enable=s;
}

bool ChemicalInjector::isEnabled()
{
    return m_enable;
}

float ChemicalInjector::remainingPurcent()
{
    return (m_volumeMl-totalInjected())/m_volumeMl;
}

float ChemicalInjector::totalInjected()
{
    return m_pump->integral();
}



void ChemicalInjector::setState(State newState)
{
    emit stateChanged(newState);
    m_state = newState;
}

void ChemicalInjector::setVolumeMl(float newVolumeMl)
{
    m_volumeMl = newVolumeMl;
}

void ChemicalInjector::setFlow(float f)
{
    m_pump->setFlow(f);
}

QString ChemicalInjector::name() const
{
    return m_name;
}

QList<RealTimeValue> ChemicalInjector::injectionHistoric()
{
    return m_pump->integralHistoric();
}




