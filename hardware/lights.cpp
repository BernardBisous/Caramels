#include "lights.h"
#include "hardware/Pinout.h"



Lights::Lights(int pin, QObject* parent):SwitchedActuator(pin,false,"Lighs 400W_XE",parent)
{

    setDataValue("Gain [m2/W]","10");
    setResult("Power[W]");
}

float Lights::computeResult(QString s)
{
    if(s=="Power")
        return 2;
    return 1;
}

LightsSpectrum::LightsSpectrum(int pwmPin, QObject *parent):Actuator("Spectrum Potentiometer",parent)
{

}

LightsUnit::LightsUnit(QObject *parent):HardwareUnit{parent}
{
    m_name="Lights";
    attachDevice(m_power=new Lights(LIGHTS_POWER_PIN,this));
    attachDevice(m_spectrum=new LightsSpectrum(LIGHTS_SPECTRUM_PIN,this));
}
