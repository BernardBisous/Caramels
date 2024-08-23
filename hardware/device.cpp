#include "device.h"
#include "qdatetime.h"
#include "qdir.h"
#include <QFile>

#define HISTO_PATH "data/"

Device::Device(QObject *parent)
    : QObject{parent},m_currentValue(0)
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

void Device::computeResults()
{
    auto l=m_metaResults.keys();
    for(int i=0;i<l.count();i++)
    {
        m_metaResults.insert(l[i],computeResult(l[i]));
    }
}

int Device::possibleParameterId()
{
    return m_paramId;
}

bool Device::acceptsParam(int id)
{
    return id==m_paramId;
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
    return HISTO_PATH+m_name.remove(" ");
}

void Device::startRecording(bool t)
{
    if(t)
        connect(this,SIGNAL(newValue(float)),this,SLOT(storeValue(float)));
    else
        disconnect(this,SIGNAL(newValue(float)),this,SLOT(storeValue(float)));
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
    if(l.isEmpty())
        applyValue(0);

    else
        applyValue(l.last().value);
}

void Device::applyValue(float v)
{
    qDebug()<<"Device new value"<<name()<<v;
    m_currentValue=v;
    emit newValue(v);
}

void Device::applyPurcent(int t)
{
    //TODO put a gain
    applyValue(t);
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

float Device::currentValue() const
{
    return m_currentValue;
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
