#include "co2manager.h"
#include "hardware/Pinout.h"

CO2Manager::CO2Manager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Gestion du CO2";
    attachDevice(m_injector=new SwitchedActuator(CO2_INJECTOR_PIN,false,"Injecteur CO2",this));
    attachDevice(m_sensor=new AnalogSensor(CO2_SENSOR_PIN,20,"Capteur de CO2",this));

    m_idParameters<<CO2_LEVEL;
}
