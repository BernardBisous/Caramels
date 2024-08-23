#include "pump.h"
#include "hardware/Pinout.h"

Pump::Pump(int pin, QObject *parent)
    :SwitchedActuator(pin,false,parent)
{
    m_paramId=INJECTION;
    m_name="Main Water Pump";
}
