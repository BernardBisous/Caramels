#include "hardwareunit.h"


HardwareUnit::HardwareUnit(QObject *parent)
    : QObject{parent},m_description("Regular hardware unit")
{

}

void HardwareUnit::begin()
{
    for(int i=0;i<m_devices.count();i++)
        m_devices[i]->begin();

   // computeEndTime();
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

    m_startTime=QDateTime();
    computeEndTime();
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

    if(m_couples.contains(p->id()))
    {
        m_couples.value(p->id())->attachParameter(p);
    }

    computeEndTime();
}

void HardwareUnit::trigKey(QString s)
{
    qDebug()<<"KeyTriggered "<<name()<<s;
}

AnalogSensor *HardwareUnit::regulatingSensor()
{

    auto l=sensors();
    for(int i=0;i<l.count();i++)
    {
        AnalogSensor*a=dynamic_cast<AnalogSensor*>(l[i]);
        if(a)
            return a;
    }
    return nullptr;
}



void HardwareUnit::attachCouples(int p, Device *e)
{
    m_couples.insert(p,e);
}

Device *HardwareUnit::attachedDevice(Parameter *p)
{
    if(!p || !m_couples.contains(p->id()))
        return nullptr;
    return m_couples.value(p->id());
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
    {
        m_sensors[i]->measure();   
    }
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



QList<Actuator *> HardwareUnit::actuators(bool integratedOnly)
{
    QList<Actuator *> out;
    for(int i=0;i<m_devices.count();i++)
    {
        Actuator*a=dynamic_cast<Actuator*> (m_devices[i]);
        if(a)
        {
            if(!integratedOnly || a->integratedInteresting())
                out<<a;
        }
    }
    return out;
}

QDateTime HardwareUnit::startTime() const
{
    return m_startTime;
}

void HardwareUnit::setStartTime(const QDateTime &newStartTime)
{
    m_startTime = newStartTime;
    computeEndTime();
    emit startChanged(m_startTime);
}

QDateTime HardwareUnit::endConfig()
{
    return m_endTime;
}

void HardwareUnit::computeEndTime()
{
    if(!m_startTime.isValid())
    {
        m_endTime=m_startTime;
        return;
    }
    int max=0;

    for( int i=0;i<m_parameters.count();i++)
    {
        int n=m_parameters[i]->maxX();
        if(!i || n>max)
            max=n;
    }

    m_endTime=m_startTime.addSecs(max*Parameter::timeMultiplicator());
}

QList<Device *> HardwareUnit::coupledDevices()
{
    return m_couples.values();
}

QString HardwareUnit::description() const
{
    return m_description;
}

void HardwareUnit::setDescription(const QString &newDescription)
{
    m_description = newDescription;
}

