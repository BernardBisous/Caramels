#include "device.h"
#include "hardware/rasppi.h"
#include "qdatetime.h"
#include "qdir.h"
#include <QFile>

#define HISTO_PATH "data/"
#define PR_SAMPLING_RATE "Freq.[s]"

#include <QRandomGenerator>

Device::Device(QString name,QObject *parent)
    : QObject{parent},m_recording(true),m_name(name),m_currentValue(0)
{

}

void Device::begin()
{
    retreiveLastValue();
    startRecording(true);
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

void Device::reactToDataEdited(QString, QString)
{

}



void Device::computeResults()
{
    auto l=m_metaResults.keys();
    for(int i=0;i<l.count();i++)
    {
        m_metaResults.insert(l[i],computeResult(l[i]));
    }
}

QString Device::dataValue(QString key)
{
    return m_metaData.value(key);
}

bool Device::existData(QString key)
{
    return m_metaData.contains(key);
}

void Device::setDataValue(QString key, QString val, bool notif)
{
    m_metaData.insert(key,val);
    if(notif)
        reactToDataEdited(key,val);
}

void Device::setResult(QString key)
{
    m_metaResults.insert(key,computeResult(key));
}

QStringList Device::resultKeys()
{
 return m_metaResults.keys();
}

QStringList Device::dataKeys()
{
    return m_metaData.keys();
}

QString Device::name() const
{
    return m_name;
}

void Device::setName(const QString &newName)
{
    m_name = newName;
}

QString Device::storageFile()
{
    return HISTO_PATH+m_name;
}

void Device::startRecording(bool t)
{
    m_recording=t;
}

QList<RealTimeValue> Device::historic()
{
    QList<RealTimeValue> output;
    QFile file(storageFile());
    if (!file.open(QIODevice::ReadOnly)) {


        return output;
    }

    QDataStream out(&file);

    while(!out.atEnd())
    {
        RealTimeValue v;
        out>>v.time;
        out>>v.value;
        output.append(v);
    }
    file.close();

    return output;
}

void Device::retreiveLastValue()
{
    auto l=historic();
    float v=0;
    if(!l.isEmpty())
        v=l.last().value;

    appendValue(v);
}



bool Device::createDataDir()
{
    QString sp(HISTO_PATH);
    sp.remove("/");
    QDir r(sp);

    if(!r.exists())
    {
        QDir d=QDir::current();
        d.mkdir(sp);
        return true;
    }
    return false;
}

void Device::storeValue(float t)
{
    QFile file(storageFile());
    if (!file.open(QIODevice::Append)) {

        return;
    }
    QDataStream out(&file);
    out << QDateTime::currentDateTime();
    out << t;
    file.close();
    return;
}

void Device::appendValue(float v)
{

    m_currentValue=v;

    if(m_recording)
        storeValue(v);

    emit newValue(v);
}

float Device::currentValue() const
{
    return m_currentValue;
}


SwitchedActuator::SwitchedActuator(int pin, bool pwm, QString name, QObject *parent)
    :Actuator(name,parent),m_pin(pin),m_pwmAnalog(pwm)
{

}

float SwitchedActuator::filterInputValue(float v)
{
    if(!m_pwmAnalog)
        return v;

    return (v>50)*100;
}

void SwitchedActuator::begin()
{
    Device::begin();
    RasPi::initPin(m_pin,RasPi::OUTPUT);
}

void SwitchedActuator::applyValue(float v, int ms)
{
    Actuator::applyValue(v,ms);
    RasPi::write(m_pin,filterInputValue(v));
}


Actuator::Actuator(QString name, QObject *parent):
    Device(name,parent)
{

}

void Actuator::applyValue(float v, int t)
{

  // float val=currentValue();
    appendValue(v);

    if(t>0)
    {
        float gain=dataValue("Speed[cm/s]").toFloat();
        int ms=t/gain;

        QTimer::singleShot(ms, [this]() {
               applyValue(0);
            });
    }


}

void Actuator::applyPurcent(int o)
{
    applyValue(o);
}

Sensor::Sensor(QString name, QObject *parent)
    :Device(name,parent),m_pollTimer(nullptr)
{
    setDataValue("Freq.[s]","1");
}

float Sensor::aquire()
{
    return 0;
}

void Sensor::reactToDataEdited(QString key, QString )
{
    if(key==PR_SAMPLING_RATE)
    {

        updateSamplingRate();
    }
}

void Sensor::begin()
{
    Device::begin();
    startPolling(true);
}

void Sensor::updateSamplingRate()
{
    float s=dataValue(PR_SAMPLING_RATE).toFloat()*1000;

    if(m_pollTimer)
        m_pollTimer->setInterval(s);
}

void Sensor::startPolling(bool s)
{

    if(s)
    {
        if(!m_pollTimer)
        {
            m_pollTimer=new QTimer(this);
            connect(m_pollTimer,SIGNAL(timeout()),this,SLOT(measure()));
        }
        updateSamplingRate();
        m_pollTimer->start();
    }
    else if(m_pollTimer)
        m_pollTimer->stop();
}

void Sensor::measure()
{
    float a=aquire();

    if(a!=m_currentValue || m_continousStreaming)
    {
        appendValue(a);

    }

}



Motor::Motor(int dirpin, int pwm, QString name, QObject *parent):
    Actuator(name,parent),m_dirPin(dirpin),m_pwmPin(pwm)
{

}

void Motor::begin()
{
    RasPi::initPin(m_dirPin,RasPi::OUTPUT);
    RasPi::initPin(m_dirPin,RasPi::OUTPUT);
}

void Motor::applyValue(float v, int ms)
{
    Actuator::applyValue(v,ms);
    bool dir=v>0;
    RasPi::write(m_dirPin,dir);
    RasPi::write(m_pwmPin,qAbs(v));
}

