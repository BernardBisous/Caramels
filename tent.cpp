#include "tent.h"

#include "hardware/co2manager.h"
#include "hardware/lights.h"
#include "hardware/temperaturemanager.h"
#include "hardware/tolleveler.h"
#include "hardware/waterlevelmanager.h"
#include "qurl.h"

#include <QDir>
#include <QDesktopServices>
#include <QSettings>
#include <QFile>

#define LOGS_FILE "Logs"
Tent::Tent(QObject *parent)
    : QObject{parent},m_config(nullptr)
{


    m_name="Tente Visionaire";
    m_id=3;

    m_serial=new SerialTent(this);


    m_cam=new Webcam(this);
    initDevices();
    m_timer=new QTimer(this);
    m_timer->setInterval(Parameter::timeMultiplicator()*1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));

    connect(m_serial,SIGNAL(newValues(QByteArray&)),this,SLOT(hardwareSlot(QByteArray&)));
    connect(m_serial,SIGNAL(connectedChanged(bool)),this,SLOT(serialConnectSlot(bool)));


    loadSetting();
}

void Tent::initDevices()
{
    Device::createDataDir();
    addUnit(m_pumps=new WaterLevelManager(this));
    addUnit(m_lights=new LightsUnit(this));
    addUnit(m_leveler=new TolLeveler(this));
    addUnit(m_temperatures=new TemperatureManager(this));
    addUnit(m_Co2=new CO2Manager(this));
    addUnit(m_ph=new PHManager(this));

    m_temperatures->setCo2(m_Co2);
}

void Tent::begin()
{

    m_serial->open(serialPort());

    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->begin();
    }


    if(m_startedDate.isValid() && m_config)
    {
        m_timer->start();
    }


}


void Tent::exportAll(QString dir)
{

    if(dir.isEmpty())
        return;

    console("exporting all at "+dir);
    QDir root(dir);
    for(int i=0;i<m_units.count();i++)
    {
        root.mkdir(m_units[i]->name());
        QDir d(root.absolutePath()+"/"+m_units[i]->name());
        m_units[i]->exportAll(d.absolutePath());
    }

    root.mkdir("Camera");
    QDir dc(root.absolutePath()+"/Camera");
    m_cam->exportAll(dc.absolutePath());



    QFile::copy(LOGS_FILE,dir+"/ReadMe.txt");

    if(m_config)
        m_config->saveCsv(dir+"/ConfigDone.csv");

    QDesktopServices::openUrl(root.absolutePath());
}


int Tent::currentHourIndex()
{
    int h=m_startedDate.secsTo(QDateTime::currentDateTime())/Parameter::timeMultiplicator();

    if(!m_config || h>m_config->maxHours())
        return -1;
    return h;
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

        if(m_units[i]->canHandleParameter(id))
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
    connect(u,SIGNAL(consoleRequest(QString)),this,SLOT(console(QString)));
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
    d->setSerial(m_serial);
    d->loadSettings();

    if(!m_devices.contains(d))
        m_devices.append(d);
}

void Tent::saveSettings()
{
    QSettings settings("YourOrganization", "TentSettings"); // Replace with your organization and application names
    settings.setValue("StartDate", m_startedDate);
    settings.setValue("SerialPort", m_serialPort);

}

void Tent::loadSetting()
{
    QSettings settings("YourOrganization", "TentSettings");
    QDateTime d = settings.value("StartDate").toDateTime();
    m_serialPort= settings.value("SerialPort").toString();

   // qDebug()<<"loaded serial"<<m_serialPort;
    if(d.isValid())
    {
        setStartDate(d);
        start();
    }
}


void Tent::restart()
{  


    QFile::remove(consoleFile());
    m_cam->clearAll();
    for(int i=0;i<m_devices.count();i++)
    {
        m_devices[i]->clearHistoric();

    }

    setStartDate(QDateTime::currentDateTime());

}

void Tent::start()
{
    if(!m_startedDate.isValid() || !m_config)
        return;

    m_cam->start();
    timerSlot();
    m_timer->start();
    for(int i=0;i<m_devices.count();i++)
    {
        m_devices[i]->startRecording(true);

    }
    console("start config "+m_config->name());
}

void Tent::setStartDate(QDateTime t)
{
    m_startedDate=t;
    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->setStartTime(m_startedDate);
    }
    saveSettings();
    start();
    emit dateChanged(t);
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

float Tent::PH()
{
    return m_ph->ph();
}

float Tent::temperature(int sensorIndex)
{
    switch(sensorIndex)
    {
    case 0: return m_temperatures->airTemperature();
    default: return m_temperatures->waterTemperature();
    }
}

float Tent::CO2()
{
    return m_Co2->CO2();
}

float Tent::humidity()
{
    return m_temperatures->humidity();
}

float Tent::lightPower()
{
    return m_lights->lightPower();
}

float Tent::lightSpectrum()
{
    return m_lights->spectrumValue();
}

QString Tent::injectingState()
{
    return m_pumps->injectingState();
}

QDateTime Tent::startedDate() const
{
    return m_startedDate;
}

QString Tent::consoleFile()
{
    return LOGS_FILE;
}

void Tent::finish()
{
    m_timer->stop();
    setStartDate(QDateTime());
    stopAll();
}

void Tent::console(QString s)
{

    qDebug()<<"Conlsole:"<<s;

    QFile file(consoleFile());
    if (!file.open(QIODevice::Append)) {
        return;
    }
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString()+" : "+s+"\n";
    file.close();
    return;
}

void Tent::hardwareSlot(QByteArray &d)
{


    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->updateSensors();
    }
    emit sensorsAquiered();
}

void Tent::serialConnectSlot(bool s)
{
    emit connectedHardware(s);
}

void Tent::timerSlot()
{

    int h=currentHourIndex();

    if(h<0)
    {
        finish();
        emit done();
        return;
    }

    if(m_serial->isConnected())
    {
        for(int i=0;i<m_units.count();i++)
            m_units[i]->update(h);
    }

    emit newValue(h);
}

Webcam *Tent::cam() const
{
    return m_cam;
}

WaterLevelManager *Tent::pumps() const
{
    return m_pumps;
}

LightsUnit *Tent::lights() const
{
    return m_lights;
}



TolLeveler *Tent::leveler() const
{
    return m_leveler;
}


TemperatureManager *Tent::temperatures() const
{
    return m_temperatures;
}

PHManager *Tent::phManager()
{
    return m_ph;
}

CO2Manager *Tent::co2Manager()
{
    return m_Co2;
}

QString Tent::serialPort() const
{
    return m_serialPort;
}

void Tent::setSerialPort(const QString &newSerialPort)
{
    m_serialPort = newSerialPort;
    m_serial->open(m_serialPort);
    saveSettings();
}

QStringList Tent::availablePorts()
{

    QStringList out;
    auto ls=QSerialPortInfo::availablePorts();
    for(int i=0;i<ls.count();i++)
    {
        out<<ls[i].portName();
    }
    return out;
}

bool Tent::connected()
{
    return m_serial->isConnected();
}

void Tent::stopAll()
{
    m_cam->stop();
    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->finish();
    }
}
