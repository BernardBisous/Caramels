#include "analogsensor.h"



AnalogSensor::AnalogSensor(int pin, float gain,QString n, QObject *parent):
    Sensor(n,parent),m_gain(gain),m_pin(pin)
{
    setDataValue("Gain",QString::number(gain));
}

float AnalogSensor::aquire()
{
    return RasPi::readAnalog(m_pin)*m_gain;

}

void AnalogSensor::reactToDataEdited(QString key, QString val)
{
    if(key=="Gain")
    {
        m_gain=val.toFloat();
    }
}
