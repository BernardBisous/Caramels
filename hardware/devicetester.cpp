#include "devicetester.h"

#define DEFAULT_DELAY 5000
DeviceTester::DeviceTester(Device *d, Tent *t, QObject *parent)
    : QObject{parent},m_device(d),
      m_actuator(dynamic_cast<Actuator*>(d)),
      m_tent(t),
      m_sensor(t->general()->sensor()),
      m_switchTimer(nullptr)
{
}

void DeviceTester::run()
{
    m_high.clear();
    m_low.clear();

    if(m_actuator)
    {
        setIntervalActuator(DEFAULT_DELAY);
        m_switchTimer->start();

    }

}

void DeviceTester::setIntervalActuator(int i)
{
    if(i>0)
    {
        if(!m_switchTimer)
        {
            m_switchTimer=new QTimer(this);
            connect(m_switchTimer,SIGNAL(timeout()),this,SLOT(timerSlot()));
        }

        m_switchTimer->setInterval(i);
    }
}

float DeviceTester::consupmtion()
{
    float avl=0;
    int i=0;
    for(;i<m_low.count();i++)
    {
        avl+=m_low[i];
    }

    if(!i)
        return 0;

    avl=avl/(i);

    float avh=0;

    for(i=0;i<m_high.count();i++)
    {
        avh+=m_high[i];
    }

    if(!i)
        return 0;

    avh=avh/(i);


    return avh-avl;
}

QString DeviceTester::subText()
{
    if(m_actuator)
        return "Vérivier que l'équipement fonctionne lorsque la valeur est haute et qu'il a a l'arret autrement";

    return "Test du capteur, essayer d'étalonner la valeur avec le plus précis étalon possible";
}

void DeviceTester::timerSlot()
{
    if(m_sensor && m_actuator)
    {
        bool s=m_actuator->currentPurcent()==m_actuator->neutralPurcent();

        float current=m_sensor->currentValue();

        if(s)
            m_high<<current;
        else
            m_low<<current;

        m_actuator->switchStateUser();
        emit newCurrent(consupmtion());
    }
}

AnalogSensor *DeviceTester::sensor() const
{
    return m_sensor;
}
