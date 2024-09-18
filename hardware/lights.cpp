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



LightsUnit::LightsUnit(QObject *parent):HardwareUnit{parent}
{
    m_name="Lampes";
    setDescription("Gestion de la puissance et du spectre de la lumière");

    attachDevice(m_power=new Lights(LIGHTS_POWER_PIN,this));
    attachDevice(m_spectrum=new LightsSpectrum(LIGHTS_SPECTRUM_PIN,this));

    m_idParameters<<LIGHTS_DAY<<LIGHTS_SLEEP<<LIGHTS_SPECTRUM;
    setTimeRegulated(m_power);
}

void LightsUnit::begin()
{
    HardwareUnit::begin();
}

void LightsUnit::regulatorSlot(bool s)
{
   if(s)
       console("Switching lights on for "+QString::number(regulator()->high()/(1000*3600))+"h");


   else
       console("Switching lights off for "+QString::number(regulator()->low()/(1000*3600))+"h");

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
       regulator()->setDelayHigh(f/3600*Parameter::timeMultiplicator());

        if(!f)
            regulator()->startState(true);

    }
    else if(p==night())
    {
        regulator()->setDelayLow(f/3600*Parameter::timeMultiplicator());
    }
}

void LightsUnit::finish()
{
    regulator()->reset();
    m_power->userApplyPurcent(0);
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


