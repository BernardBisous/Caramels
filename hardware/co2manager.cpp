#include "co2manager.h"
#include "hardware/Pinout.h"

#define CO2_TANK_ML 2000
#define CO2_FLOW 100//mL/s
CO2Manager::CO2Manager(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="CO2";

    m_injector=new ChemicalInjector(NO_PIN,CO2_INJECTOR_PIN,NO_PIN,CO2_LEVEL,this);
    m_valve=m_injector->pump();
    m_injector->enable(true);
    m_injector->setName("CO2");

    //Should not erease settings !
    m_injector->setVolumeMl(CO2_TANK_ML);
    m_injector->setFlow(CO2_FLOW);

    m_valve->setName("Injecteur CO2");

    attachDevice(m_valve);
    attachDevice(m_sensor=new AnalogSensor(CO2_SENSOR_PIN,"Capteur de CO2",this));

    m_idParameters<<CO2_LEVEL;
    attachCouples(CO2_LEVEL,m_sensor);

    m_sensor->setRange(0,5000);
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


