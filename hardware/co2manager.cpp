#include "co2manager.h"
#include "hardware/Pinout.h"

CO2Manager::CO2Manager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Gestion du CO2";
    attachDevice(m_injector=new SwitchedActuator(CO2_INJECTOR_PIN,false,"Injecteur CO2",this));
    attachDevice(m_sensor=new AnalogSensor(CO2_SENSOR_PIN,"Capteur de CO2",this));

    m_idParameters<<CO2_LEVEL;

    attachCouples(CO2_LEVEL,m_sensor);

    m_sensor->setRange(100,1000);
    m_sensor->setUnits("ppm");

    m_injector->setIntegralUnits("m3");
    m_injector->setRange(0,50);
    m_injector->setUnits("ml/s");

    setDescription(" En charge de la gestion du CO2 dans l'atmoshpère (et autre gazs si nécéssaire)");
}

void CO2Manager::reactToParamChanged(Parameter *, float v)
{
    m_command=v;
    m_sensor->setCommand(v);
    reactToSensorsChanged();
}

void CO2Manager::reactToSensorsChanged()
{
    if(m_sensor->shouldRegulate())
    {


        float err=excess();
        console("Regulating CO2 : value:"+QString::number(CO2())+" Parameter: "+QString::number(m_command)+"ppm");

        if(err<0)
        {
            //m_injector->impulseHigh(err*m_injector->gain()*1000);
            m_injector->impulseHigh(1000);
        }
        m_sensor->setRegulated();
    }
}

float CO2Manager::CO2()
{
    return m_sensor->currentValue();
}

float CO2Manager::excess()
{
    return CO2()-m_command;
}


