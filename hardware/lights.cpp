#include "lights.h"
#include "hardware/Pinout.h"




Lights::Lights(int pin, QObject* parent):SwitchedActuator(pin,false,"Lighs 400W_XE",parent)
{
    setUnits("W");
    setRange(0,300);//Total lights power
    setIntegralUnits("Ws");
}

float Lights::computeResult(QString s)
{
    if(s=="Power")
        return 2;
    return 1;
}

LightsSpectrum::LightsSpectrum(int pwmPin, QObject *parent):
    SwitchedActuator(pwmPin,true,"Spectrum Potentiometer",parent)
{
    setRange(400,700);//Total lights power
    setUnits("Nm");
}



LightsUnit::LightsUnit(QObject *parent):HardwareUnit{parent},m_delayNight(0),m_delayDay(0)
{
    m_name="Lampes";
    setDescription("Gestion de la puissance et du spectre de la lumière");
    m_switchTimer=new QTimer(this);
    connect(m_switchTimer,SIGNAL(timeout()),this,SLOT(switchLights()));

    attachDevice(m_power=new Lights(LIGHTS_POWER_PIN,this));
    attachDevice(m_spectrum=new LightsSpectrum(LIGHTS_SPECTRUM_PIN,this));

    m_idParameters<<LIGHTS_DAY<<LIGHTS_SLEEP<<LIGHTS_SPECTRUM;
}

void LightsUnit::begin()
{
    HardwareUnit::begin();


   // switchLights();
}

void LightsUnit::switchLights()
{
    bool b=m_power->currentValue()>0;

    if(b)
    {
        console("Switching lights off for "+QString::number(m_delayNight/(1000*3600))+"h");
        m_switchTimer->setInterval(m_delayNight);
        m_power->userApplyPurcent(0);
    }
    else
    {
         console("Switching lights on for "+QString::number(m_delayDay/(1000*3600))+"h");
        m_switchTimer->setInterval(m_delayDay);
        m_power->userApplyPurcent(100);
    }

    m_switchTimer->start();

}

void LightsUnit::reactToParamChanged(Parameter *p, float f)
{
    if(p==spectrum())
    {
        updateSpectrum(f);
        return;
    }
    else if(p==day())
    {
        bool bl=m_delayDay;

        m_delayDay=f*1000*Parameter::timeMultiplicator();

        if(!bl)
            switchLights();
    }
    else if(p==night())
    {
        m_delayNight=f*1000*Parameter::timeMultiplicator();
    }
}

void LightsUnit::finish()
{

    m_delayDay=0;
    m_delayNight=0;
    m_switchTimer->stop();
    m_power->applyPurcent(0);
    HardwareUnit::finish();
}

bool LightsUnit::isDayLight()
{
    return m_power->currentValue();
}

QList<Device *> LightsUnit::interestingDevices()
{
    return m_devices;
}

QList<Actuator *> LightsUnit::interestingIntegrals()
{
    return QList<Actuator *> ()<<m_power;
}

void LightsUnit::updateSpectrum(float t)
{
    console("Setting light sprectrum to "+m_spectrum->userValue());
    m_spectrum->applyValue(t);
}

float LightsUnit::lightPower()
{
    return m_power->currentValue();
}

float LightsUnit::spectrumValue()
{
    return m_spectrum->currentValue();
}


