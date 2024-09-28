#include "analogsensor.h"


#define SMOOTH_KEY "Smoothing"
#define REG_KEY "Regulation"
#define WATCHDOG_INDEX 3

AnalogSensor::AnalogSensor(int pin, QString n, QObject *parent):
    Sensor(pin,n,parent),m_lastRegTime(),m_consecutiveErrors(0)
{
    m_maxValues=100;
    setSmoothing(5);
    setRegulationDelay(Parameter::timeMultiplicator());
    setRecordDelay(Parameter::timeMultiplicator());

    m_continousStreaming=true;
}

void AnalogSensor::setSmoothing(int values)
{
    setDataValue(SMOOTH_KEY,QString::number(values));
}

int AnalogSensor::smoothing() const
{
    return dataValue(SMOOTH_KEY).toInt();
}

int AnalogSensor::regulationDelay()
{
    return dataValue(REG_KEY).toInt();
}

void AnalogSensor::setRegulationDelay(int seconds)
{

    setDataValue(REG_KEY,QString::number(seconds));
}

bool AnalogSensor::shouldRegulate()
{    
    if(!m_lastRegTime.isValid() || !nextRegulation().isValid())
        return true;

    return (nextRegulation()<QDateTime::currentDateTime());
}

void AnalogSensor::begin()
{
    Sensor::begin();
    m_lastRegTime=QDateTime();
}

QDateTime AnalogSensor::nextRegulation()
{
    int n=regulationDelay();

    if(!m_lastRegTime.isValid() || n<=0 )
        return QDateTime();


    QDateTime t=m_lastRegTime.addSecs(n);
    return t;
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
    m_lastRegTime=QDateTime();
    Sensor::reset();
}

void AnalogSensor::regulateNow()
{
    m_lastRegTime=QDateTime();
    measure();
}

void AnalogSensor::setRegulated()
{
    m_lastRegTime=QDateTime::currentDateTime();
    emit regulated();
}

void AnalogSensor::measure()
{
    float a=aquire();

    if(a<=0)
    {

        m_consecutiveErrors++;

        if(m_consecutiveErrors==WATCHDOG_INDEX)
        {
            m_consecutiveErrors++;
            emit error("Signal nul",false);
        }
        return;
    }

    if(m_consecutiveErrors)
    {
        emit error("",false);
        m_consecutiveErrors=0;
    }


    m_smoothedValues.append(a);
    int n=smoothing();

    while(n<m_smoothedValues.count())
        m_smoothedValues.takeFirst();

    appendValue(smoothedValue());
}

float AnalogSensor::command() const
{
    return m_command;
}

void AnalogSensor::setCommand(float newCommand)
{
    m_command = newCommand;
}

float AnalogSensor::errorValue()
{
    return currentValue()-m_command;
}

QString AnalogSensor::userValue()
{
    if(m_consecutiveErrors)
          return "No signal";

    QString err=" ("+QString::number(errorValue(),'f',1)+")";
    return Sensor::userValue()+err;
}



