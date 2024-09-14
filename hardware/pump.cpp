#include "pump.h"


Pump::Pump(int pin, QString name,QObject *parent)
    :SwitchedActuator(pin,false,name,parent)

{
    setUnits("mL/s");
    setIntegralUnits("ml");
}

void Pump::inject(float volumeML)
{
    int ms=volumeML*gain()*1000;

   // qDebug()<<"pumpn injectr "<<m_name<<volumeML<<ms;
    impulseHigh(ms);
}

void Pump::startInjecting(bool s)
{
    if(s)
    {
        userApplyPurcent(100);
    }
    else
    {

        userApplyPurcent(0);
    }

}

void Pump::setFlow(float t)
{
    setRange(0,t);
}



