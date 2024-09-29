#include "tolleveler.h"

#define REGULATE false
TolLeveler::TolLeveler(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Ascenceur";
    attachDevice(m_lifter=new TopLifter(HEIGH_LIFTER_UP,HEIGH_LIFTER_DOWN,"Ascenseur",this));
    attachDevice(m_sensor=new DistanceSensor(HEIGH_SENSE,"Capteur de hauteur",this));

    m_sensor->setUnits("cm");
    m_sensor->setRange(0,1000);
    m_lifter->setUnits("cm/s");
    m_lifter->setMaxSpeed(2);//cm/s

    m_lifter->setIntegratedInteresting(true);
    m_lifter->setIntegralUnits("cm");

    setDescription("Gestion de la hauteur des lampes via un ascenseur");

    m_idParameters<<LIGHT_HEIGH;

    if(REGULATE)
        attachCouples(LIGHT_HEIGH,m_sensor);
}


QList<Device *> TolLeveler::interestingDevices()
{
    return m_devices;
}

QList<Actuator *> TolLeveler::interestingIntegrals()
{
    return QList<Actuator *> ()<<m_lifter;
}

void TolLeveler::reactToParamChanged(Parameter *p, float v)
{
    if(p==heigh())
    {
        if(REGULATE)
            m_sensor->setCommand(v);
        else
        {

            m_lifter->moveTo(v);
        }

        reactToSensorsChanged();
    }

}

void TolLeveler::reactToSensorsChanged()
{
    if(REGULATE && m_sensor->shouldRegulate())
    {
        float err=m_sensor->errorValue();
        console("Regulating heigh : "+m_sensor->userValue());
        m_lifter->move(-err);
        m_sensor->setRegulated();
    }
}

float TolLeveler::heighValue()
{
    return m_sensor->currentValue();
}


TopLifter::TopLifter(int dir, int pwm, QString name, QObject *parent):
    Motor(dir,pwm,name,parent)
{

}


