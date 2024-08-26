#include "tent.h"
#include "Interface/windmanager.h"
#include "hardware/Pinout.h"
#include "hardware/chemicalinjector.h"
#include "hardware/co2manager.h"
#include "hardware/lights.h"
#include "hardware/temperaturemanager.h"
#include "hardware/tolleveler.h"
#include "hardware/waterlevelmanager.h"

#include <QSettings>
Tent::Tent(QObject *parent)
    : QObject{parent},m_config(nullptr)
{

    m_name="Tente 1";
    m_id=3;
    loadSetting();
    initDevices();

}

void Tent::initDevices()
{

    Device::createDataDir();
    addUnit(new WaterLevelManager(this));
    addUnit(new LightsUnit(this));
    addUnit(new TolLeveler(this));
    addUnit(new TemperatureManager(this));
    addUnit(new WindManager(this));
    addUnit(new CO2Manager(this));


    addUnit(new ChemicalInjector(CHEM_MIX_1_PIN,CHEM_PUMP_1_PIN,CHEM_LEVEL_PIN,0,this));
    addUnit(new ChemicalInjector(CHEM_MIX_2_PIN,CHEM_PUMP_2_PIN,CHEM_LEVEL_PIN,1,this));
    addUnit(new ChemicalInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,CHEM_LEVEL_PIN,2,this));


}

void Tent::begin()
{
    RasPi::begin();

    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->begin();
    }

    restart();
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
        if(m_units[i]->idParameters().contains(id))
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

             d->attachParameter(p);
        }
    }
}

void Tent::addUnit(HardwareUnit *u)
{
    m_units.append(u);
    u->setStartTime(m_startedDate);
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

void Tent::saveSettings()
{
    QSettings settings("YourOrganization", "YourApplication"); // Replace with your organization and application names
    settings.setValue("StartDate", m_startedDate.toString());
}

void Tent::loadSetting()
{
    QSettings settings("YourOrganization", "YourApplication");
    QString startDateString = settings.value("StartDate").toString();


}


void Tent::restart()
{
    m_startedDate=QDateTime::currentDateTime();
    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->setStartTime(m_startedDate);
    }
    saveSettings();

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
