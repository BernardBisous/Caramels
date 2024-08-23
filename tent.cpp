#include "tent.h"
#include "hardware/Pinout.h"
#include "hardware/lights.h"
#include "hardware/pump.h"

Tent::Tent(QObject *parent)
    : QObject{parent},m_config(nullptr)
{

    m_name="Tente 1";
    m_id=3;
    initDevices();
}

void Tent::initDevices()
{
    addDevice(new Lights(LIGHTS_POWER_PIN,this));
    addDevice(new LightsSpectrum(this));
    addDevice(new Pump(MAIN_PUMP_PIN ,this));
}

void Tent::begin()
{
    for(int i=0;i<m_devices.count();i++)
    {
        m_devices[i]->begin();
    }
}

QString Tent::configName() const
{
    return m_configName;
}

void Tent::setConfig(GrowConfig *e)
{
    m_config=e;
    mapDevices();
}

Device *Tent::deviceForId(int id)
{
    for(int i=0;i<m_devices.count();i++)
    {
        if(m_devices[i]->acceptsParam(id))
            return m_devices[i];
    }
    return nullptr;
}

void Tent::mapDevices()
{
    for(int i=0;i<m_config->countParameters();i++)
    {

        Parameter* p=m_config->parameterAddr(i);

        Device*d=deviceForId(p->id());
        if(d)
        {
            p->attach(d);
        }
    }
}

void Tent::addDevice(Device *d)
{

    m_devices.append(d);
}


void Tent::restart()
{
    m_startedDate=QDateTime::currentDateTime();
}

GrowConfig *Tent::config() const
{
    return m_config;
}

QString Tent::name() const
{
    return m_name;
}

void Tent::setName(const QString &newName)
{
    m_name = newName;
}

QList<Device *> Tent::devices() const
{
    return m_devices;
}

int Tent::indexOf(Device *s)
{
    return m_devices.indexOf(s);
}
