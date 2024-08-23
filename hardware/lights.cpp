#include "lights.h"
#include "hardware/Pinout.h"


Lights::Lights(int pin, QObject* parent):SwitchedActuator(pin,false,parent)
{
    m_paramId=LIGHTS_POWER;
    m_name="Lighs 400W_XE";

    setDataValue("Gain [m2/W]","10");
}

LightsSpectrum::LightsSpectrum(QObject *parent):Actuator(parent)
{
    m_paramId=LIGHTS_SPECTRUM;
    m_name="Spectrum Potentiometer";


}
