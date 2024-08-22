#include "device.h"

Device::Device(QObject *parent)
    : QObject{parent},m_client(nullptr)
{

}

void Device::begin()
{

}

void Device::save(QDataStream &s)
{
    s<<m_metaData;
}

void Device::load(QDataStream &s)
{
    m_metaData.clear();
    s>>m_metaData;
}

int Device::possibleParameterId()
{
    return m_paramId;
}

void Device::attach(Parameter *p)
{
    m_client=p;
}

Parameter *Device::client() const
{
    return m_client;
}

QString Device::dataValue(QString key)
{
    return m_metaData.value(key);
}

bool Device::existData(QString key)
{
    return m_metaData.contains(key);
}

void Device::setDataValue(QString key, QString val)
{
    m_metaData.insert(key,val);
}


SwitchedActuator::SwitchedActuator(int pin, bool pwm, QObject *parent):Actuator(parent),m_pin(pin),m_pwmAnalog(pwm)
{

}

Actuator::Actuator(QObject *parent):Device(parent)
{

}

Sensor::Sensor(QObject *parent):Device(parent)
{

}
