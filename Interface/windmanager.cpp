#include "windmanager.h"
#include "hardware/Pinout.h"

WindManager::WindManager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Vents et tempêtes";
    attachDevice(m_power=new SwitchedActuator(WIND_POWER_PIN,10,"Ventilateur",this));
    attachDevice(m_power=new SwitchedActuator(WIND_ROTATION_PIN,10,"Orienteur du vent",this));
    m_idParameters<<WIND_LEVEL<<WIND_ROTATION;
}
