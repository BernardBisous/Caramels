#include "hardwareunit.h"

HardwareUnit::HardwareUnit(QObject *parent)
    : QObject{parent}
{
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));

}

void HardwareUnit::begin()
{
    for(int i=0;i<m_devices.count();i++)
        m_devices[i]->begin();

}

void HardwareUnit::attachDevice(Device *d)
{
     m_devices.append(d);
}

void HardwareUnit::attachParameter(Parameter *p)
{
    if(!p)
        return;
    m_parameters.append(p);
}



Device *HardwareUnit::devFromName(QString s)
{
    for(int i=0;i<m_devices.count();i++)
    {
        if(m_devices[i]->name()==s)
            return m_devices[i];
    }
    return nullptr;

}

Parameter *HardwareUnit::paramFromName(QString s)
{
    for(int i=0;i<m_parameters.count();i++)
    {
        if(m_parameters[i]->name()==s)
            return m_parameters[i];
    }
    return nullptr;
}

QStringList HardwareUnit::parametersName()
{
    QStringList out;
    for(int i=0;i<m_parameters.count();i++)
    {
        out<<m_parameters[i]->name();
    }
    return out;
}

Parameter *HardwareUnit::parameterFromId(int id)
{
    for(int i=0;i<m_parameters.count();i++)
    {
        if(m_parameters[i]->id()==id)
            return m_parameters[i];
    }
    return nullptr;
}




QList<Device *> HardwareUnit::devices() const
{
    return m_devices;
}

QList<Parameter *> HardwareUnit::parameters() const
{
    return m_parameters;
}

QString HardwareUnit::name() const
{
    return m_name;
}

QList<int> HardwareUnit::idParameters() const
{
    return m_idParameters;
}

QDateTime HardwareUnit::startTime() const
{
    return m_startTime;
}

void HardwareUnit::setStartTime(const QDateTime &newStartTime)
{
    m_startTime = newStartTime;
}

void HardwareUnit::timerSlot()
{
    update();
}


