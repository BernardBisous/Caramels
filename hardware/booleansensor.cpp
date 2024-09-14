#include "booleansensor.h"
BooleanSensor::BooleanSensor(int pin, QString n, QObject *parent)
    : Sensor{pin,n,parent},m_activeHigh(true)
{
    setRecordDelay(-1);
    m_continousStreaming=false;
}

QString BooleanSensor::userValue()
{
    bool s=(currentValue()>0);

    if(!m_activeHigh)
        s=!s;

    if(s)
        return "Vrai";
    else
        return "Faux";

}

bool BooleanSensor::activeHigh() const
{
    return m_activeHigh;
}

void BooleanSensor::setActiveHigh(bool newActiveHigh)
{
    m_activeHigh = newActiveHigh;
}

