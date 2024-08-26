#include "distancesensor.h"

DistanceSensor::DistanceSensor(int e, int t,QString name, QObject *parent)
    : Sensor{name,parent},m_echoPin(e),m_trigPin(t)
{
    setDataValue("Gain","2");
    setDataValue("Smooth","5");


    m_poller=new DistancePoller;
    connect(m_poller,SIGNAL(distanceMeasured(float)),this,SLOT(addMeasurment(float)));
}


void DistanceSensor::begin()
{

    RasPi::initPin(m_echoPin,RasPi::INPUT);
    RasPi::initPin(m_trigPin,RasPi::OUTPUT);
    Sensor::begin();
}

void DistanceSensor::reactToDataEdited(QString key, QString val)
{
    if(key=="Smooth")
        m_maxSize=val.toInt();
}

float DistanceSensor::gain()
{
    return dataValue("Gain").toFloat();
}



void DistanceSensor::measure()
{

    m_poller->startPolling(m_echoPin, m_trigPin, m_maxSize);

}

void DistanceSensor::addMeasurment(float dist)
{
    appendValue(dist);
}
