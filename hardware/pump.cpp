#include "pump.h"
#define KEY "Flow[mL/s]"
Pump::Pump(int pin, QString name,QObject *parent)
    :SwitchedActuator(pin,false,name,parent)
{
    setDataValue(KEY,"8.3");
}

void Pump::inject(float volumeML)
{
    impulseHigh(volumeML*flow()*1000);
}

float Pump::flow()
{
    return dataValue(KEY).toFloat();
}

