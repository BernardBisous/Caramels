#include "analogsensor.h"


#define SMOOTH_KEY "Smoothing"
#define REG_KEY "Regulation"
AnalogSensor::AnalogSensor(int pin, QString n, QObject *parent):
    Sensor(pin,n,parent)
{
    setSmoothing(10);
    setRegulationDelay(10);
}

void AnalogSensor::setSmoothing(int values)
{

     setDataValue(SMOOTH_KEY,QString::number(values));
}

int AnalogSensor::smoothing() const
{
    return dataValue(SMOOTH_KEY).toInt();
}

void AnalogSensor::setRegulationDelay(int seconds)
{
    setDataValue(REG_KEY,QString::number(seconds));
}

bool AnalogSensor::shouldRegulate()
{
    int s=dataValue(REG_KEY).toInt();
    if(s>0)
    {
        QDateTime now=QDateTime::currentDateTime();
        if(!m_lastRegTime.isValid() || m_lastRegTime.secsTo(now)>s)
        {
            m_lastRegTime=now;
            return true;
        }

    }
    return false;
}

float AnalogSensor::smoothedValue() const
{
    int n=m_smoothedValues.count();

    if(!n)
        return currentValue();

    float out=0;
    for(int i=0;i<n;i++)
        out+=m_smoothedValues[i];

    return out/m_smoothedValues.count();

}

void AnalogSensor::reset()
{
    m_smoothedValues.clear();
    Sensor::reset();
}

void AnalogSensor::measure()
{
    float a=aquire();
    m_smoothedValues.append(a);
    int n=smoothing();
    while(n<m_smoothedValues.count())
        m_smoothedValues.takeFirst();

    appendValue(smoothedValue());
}



