#include "co2manager.h"
#include "hardware/Pinout.h"

CO2Manager::CO2Manager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Gestion du CO2";

    m_injector=new ChemicalInjector(NO_PIN,CO2_INJECTOR_PIN,NO_PIN,CO2_LEVEL,this);
    m_valve=m_injector->pump();
    attachDevice(m_valve);
    attachDevice(m_sensor=new AnalogSensor(CO2_SENSOR_PIN,"Capteur de CO2",this));

    m_idParameters<<CO2_LEVEL;

    attachCouples(CO2_LEVEL,m_sensor);

    m_sensor->setRange(100,1000);
    m_sensor->setUnits("ppm");


    m_valve->setIntegralUnits("m3");
    m_valve->setRange(0,50);
    m_valve->setUnits("ml/s");

    setDescription(" En charge de la gestion du CO2 dans l'atmoshpère (et autre gazs si nécéssaire)");
}

void CO2Manager::reactToParamChanged(Parameter *, float v)
{

    m_sensor->setCommand(v);
    reactToSensorsChanged();
}

void CO2Manager::reactToSensorsChanged()
{
    if(m_sensor->shouldRegulate())
    {


        float err=excess();
        console("Regulating CO2 : "+m_sensor->userValue());

        if(err<0)
        {
            //m_injector->impulseHigh(err*m_injector->gain()*1000);
            m_injector->injectMl(100);
        }
        m_sensor->setRegulated();
    }
}

QList<Device *> CO2Manager::interestingDevices()
{
    return QList<Device *>()<<m_sensor<<m_valve;
}

float CO2Manager::CO2()
{
    return m_sensor->currentValue();
}

float CO2Manager::excess()
{
    return CO2()-m_sensor->command();
}

ChemicalInjector *CO2Manager::injector() const
{
    return m_injector;
}


