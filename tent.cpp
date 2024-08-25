#include "tent.h"
#include "hardware/Pinout.h"
#include "hardware/chemicalinjector.h"
#include "hardware/lights.h"
#include "hardware/waterlevelmanager.h"

Tent::Tent(QObject *parent)
    : QObject{parent},m_config(nullptr)
{

    m_name="Tente 1";
    m_id=3;
    initDevices();

}

void Tent::initDevices()
{

    Device::createDataDir();
    addUnit(new WaterLevelManager(this));
    addUnit(new LightsUnit(this));
   //addUnit(new ChemicalInjector(CHEM_MIX_1_PIN,CHEM_PUMP_1_PIN,CHEM_LEVEL_1_PIN,0,this));


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

HardwareUnit *Tent::unitForId(int id)
{
    for(int i=0;i<m_units.count();i++)
    {
        if(m_units[i]->parameterPossibleId()==id)
            return m_units[i];
    }
    return nullptr;
}



void Tent::mapDevices()
{
    for(int i=0;i<m_config->countParameters();i++)
    {

        Parameter* p=m_config->parameterAddr(i);

        HardwareUnit*d=unitForId(p->id());
        if(d)
        {


        }
    }
}

void Tent::addUnit(HardwareUnit *u)
{
    m_units.append(u);
    addDevice(u->devices());
}

void Tent::addDevice(QList<Device *> l)
{
    for(int i=0;i<l.count();i++)
    {
        addDevice(l[i]);
    }
}

void Tent::addDevice(Device *d)
{
    if(!m_devices.contains(d))
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

int Tent::indexOfDevice(Device *s)
{

    return m_devices.indexOf(s);
}

int Tent::indexOf(HardwareUnit *u)
{
    return m_units.indexOf(u);
}

QList<HardwareUnit *> Tent::units() const
{
    return m_units;
}
