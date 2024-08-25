#include "lights.h"



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

LightsSpectrum::LightsSpectrum(QObject *parent):Actuator("Spectrum Potentiometer",parent)
{

}
