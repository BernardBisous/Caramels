#include "pump.h"

Pump::Pump(int pin, QString name,QObject *parent)
    :SwitchedActuator(pin,false,name,parent)
{

}

ChemicalPump::ChemicalPump(int pin, QString name, QObject *parent)
    :SwitchedActuator(pin,false,name,parent)
{
    setDataValue("Flow[mL/s]","1.5");
}
