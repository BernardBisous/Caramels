#include "lights.h"
#include "hardware/Pinout.h"



Lights::Lights(int pin, QObject* parent):SwitchedActuator(pin,false,"Lighs 400W_XE",parent)
{
    setUnits("W");
    setRange(0,150);//Total lights power

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



LightsUnit::LightsUnit(QObject *parent):HardwareUnit{parent}
{
    m_name="Lights";
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
        m_switchTimer->setInterval(m_delayNight);
        m_power->userApplyPurcent(0);
    }
    else
    {
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

        m_delayDay=f*100;
        if(!m_switchTimer->isActive())
            switchLights();
    }
    else if(p==night())
    {
        m_delayNight=f*100;
    }
}

void LightsUnit::finish()
{
    m_switchTimer->stop();
    m_power->applyPurcent(0);
    HardwareUnit::finish();
}

void LightsUnit::updateSpectrum(float t)
{
   // qDebug()<<"updateSpectrum"<<t;
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


