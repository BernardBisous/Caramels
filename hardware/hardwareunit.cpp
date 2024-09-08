#include "hardwareunit.h"


HardwareUnit::HardwareUnit(QObject *parent)
    : QObject{parent}
{

}

void HardwareUnit::begin()
{
    for(int i=0;i<m_devices.count();i++)
        m_devices[i]->begin();


}

void HardwareUnit::update(int index)
{
    bool atEnd=true;
    for(int i=0;i<m_parameters.count();i++)
    {
        bool ok;
        float v=m_parameters[i]->valueAtTime(index,&ok);

        if(ok)
        {
            atEnd=false;
            reactToParamChanged(m_parameters[i],v);
        }
    }

    if(atEnd)
    {
        emit finished();
    }
}

void HardwareUnit::finish()
{
    for(int i=0;i<m_devices.count();i++)
        m_devices[i]->reset();
}

bool HardwareUnit::activeConfig()
{
    return m_startTime.isValid();
}

void HardwareUnit::attachDevice(Device *d)
{
    if(!d)
        return;
    Sensor* s=dynamic_cast<Sensor*>(d);
    if(s)
        m_sensors.append(s);

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

void HardwareUnit::exportAll(QString dir)
{
    for(int i=0;i<m_devices.count();i++)
    {
        m_devices[i]->exportHistoric(dir);
    }
}

bool HardwareUnit::canHandleParameter(int id)
{
    return(m_idParameters.contains(id));


}

void HardwareUnit::updateSensors()
{
    for(int i=0;i<m_sensors.count();i++)
        m_sensors[i]->measure();

    reactToSensorsChanged();
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

QList<Sensor *> HardwareUnit::sensors()
{
    return m_sensors;
}

QDateTime HardwareUnit::startTime() const
{
    return m_startTime;
}

void HardwareUnit::setStartTime(const QDateTime &newStartTime)
{
    m_startTime = newStartTime;
}




