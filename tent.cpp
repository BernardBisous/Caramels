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
#define RESULTS_FILE "Results"

#define SEND_EMAILS false
#define EMAIL_DELAYMS 20000

#include "Interface/emailnotifier.h" //TODO spagetti ???

Tent::Tent(QObject *parent)
    : QObject{parent},m_config(nullptr),
      m_general(nullptr),m_temperatures(nullptr),
      m_ph(nullptr),m_Co2(nullptr),
      m_leveler(nullptr),m_lights(nullptr),
      m_pumps(nullptr), m_chemichals(nullptr)
{



    m_name="Sublime";
    m_id=3;

    m_state=new StateNotifier(this);
    connect(m_state,SIGNAL(stateChanged()),this,SLOT(errorStateSlot()));

    m_serial=new SerialTent(this);
    m_state->append(new SerialState(m_serial,this));

    m_cam=new Webcam(this);
    m_cam->setEnabled(true);
   // m_state->append(new CamState(m_cam,this));

    initDevices();
    m_timer=new QTimer(this);
    m_timer->setInterval(Parameter::timeMultiplicator()*1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    connect(m_cam,SIGNAL(saved(QString)),this,SLOT(camCaptureSlot(QString)));
    connect(m_serial,SIGNAL(newValues(QByteArray&)),this,SLOT(hardwareSlot(QByteArray&)));
    connect(m_serial,SIGNAL(connectedChanged(bool)),this,SLOT(serialConnectSlot(bool)));

    loadSetting();
}

void Tent::initDevices()
{
    Device::createDataDir();
    addUnit(m_pumps=new WaterLevelManager(this));
    addUnit(m_chemichals=new ChemicalManager(this));
    addUnit(m_lights=new LightsUnit(this));
  //  addUnit(m_leveler=new TolLeveler(this));
    addUnit(m_temperatures=new TemperatureManager(this));
    addUnit(m_Co2=new CO2Manager(this));
    addUnit(m_ph=new PHManager(this));
    addUnit(m_general=new GeneralManager(this));

    if(m_temperatures && m_Co2)
        m_temperatures->setCo2(m_Co2);

    if(m_pumps)
        connect(m_pumps,SIGNAL(fillingTank(bool)),this,SLOT(tankFilledSlot(bool)));

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

    timerSlot();
    m_cam->start();

    if(SEND_EMAILS)
        QTimer::singleShot(EMAIL_DELAYMS, this,SLOT(sendStartupEmail()));
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

float Tent::height()
{
    if(!m_leveler)
        return-1;

    return m_leveler->heighValue();
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
    m_state->append(u);
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
    finish();
    clearAllData();
    console("start config "+m_config->name());
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

void Tent::clearAllData()
{
    QFile::remove(consoleFile());
    m_cam->clearAll();
    //clearResults();
    for(int i=0;i<m_devices.count();i++)
    {
        m_devices[i]->clearHistoric();
    }
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

QList<HardwareUnit *> Tent::unitsForParameter(Parameter *p)
{
    QList<HardwareUnit *> out;
    for(int i=0;i<m_units.count();i++)
        if(m_units[i]->parameters().contains(p))
            out<<m_units[i];

    return out;
}

float Tent::PH()
{
    if(m_ph)
    return m_ph->ph();

    return -1;
}

float Tent::temperature(int sensorIndex)
{
    if(!m_temperatures)
        return -1;
    switch(sensorIndex)
    {
    case 0: return m_temperatures->airTemperature();
    default: return -1;
    }
}

float Tent::CO2()
{
    if(!m_Co2)
        return -1;
    return m_Co2->CO2();
}

float Tent::humidity()
{
    if(m_temperatures)
    return m_temperatures->humidity();

    return -1;
}

float Tent::lightPower()
{
    if(m_lights)
        return m_lights->lightPower();

    return -1;

}

float Tent::lightSpectrum()
{
     if(m_lights)
        return m_lights->spectrumValue();

     return -1;
}



QDateTime Tent::startedDate() const
{
    return m_startedDate;
}

QString Tent::consoleFile()
{
    return LOGS_FILE;
}

QString Tent::allConsole()
{

    QFile file(consoleFile());
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }
    QTextStream out(&file);
    QString s= out.readAll();

    file.close();
    return s;
}

void Tent::finish()
{
    m_timer->stop();
    m_cam->capture();
    setStartDate(QDateTime());
    stopAll();
    console("Finished config");
}

void Tent::console(QString s)
{
    QFile file(consoleFile());
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
        return;
    }
    QTextStream out(&file);
    QString t=QDateTime::currentDateTime().toString("dd.MM hh:mm")+" "+s+"\n";
    out << t ;
    file.close();

    emit consoleRequest(t);
}

void Tent::hardwareSlot(QByteArray &)
{
    for(int i=0;i<m_units.count();i++)
    {
        m_units[i]->updateSensors();
    }
    emit sensorsAquiered();
}

void Tent::serialConnectSlot(bool s)
{

  //  updateInternalColor();
    emit connectedHardware(s);
}

void Tent::camCaptureSlot(QString s)
{
    console("Wecamp pix save"+s.right(10));
}

void Tent::errorStateSlot()
{
    updateInternalColor();
}

void Tent::sendStartupEmail()
{
    QString ps="Ps: Est-ce que ces emails sont trop fréquents ? 🤩";
    EmailNotifier::starts(m_state,ps);
    console("Sent startup email");
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

    if(m_startedDate.isValid())
    {
        for(int i=0;i<m_units.count();i++)
            m_units[i]->update(h);
    }



    if( m_cam->shouldCapture() && (!m_lights || m_lights->isDayLight() ))
        m_cam->capture();



   // storeResults();
    emit newValue(h);
}

void Tent::tankFilledSlot(bool s)
{
    if(s && m_chemichals)
        m_chemichals->fillTank();

    //storeResults();
}

StateNotifier *Tent::state() const
{
    return m_state;
}

void Tent::setInternalColorId(int id)
{
    m_serial->write(INTERNAL_LED_PIN,id);
}

void Tent::updateInternalColor()
{
    if(m_state->criticity()==DeviceState::Danger)
        setInternalColorId(1);

    else
        setInternalColorId(0);
}



ChemicalManager *Tent::chemichals() const
{
    return m_chemichals;
}

QList<ChemicalInjector *> Tent::injectors()
{
    QList<ChemicalInjector *> out;
    if(m_ph)
        out<<m_ph->injectors();

    if(m_chemichals)
        out<<m_chemichals->injectors();

    return out;
}

void Tent::setEventsDone()
{
    if(m_startedDate.isValid())
    {
        console("All events done");
        finish();
    }
}
/*
void Tent::storeResults()
{
    Result t=currentResult();
    storeResult(t);

}

void Tent::storeResult(Result r)
{
    QFile file(RESULTS_FILE);
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
         qDebug()<<"cannot store resutls"<<RESULTS_FILE;
        return;
    }

    QDataStream out(&file);
    auto l=r.values;
    for(int i=0;i<l.count();i++)
    {
        out<<l[i];
    }


    file.close();
    return;
}

Result Tent::currentResult()
{
    Result out;
    out.t=QDateTime::currentDateTime();
    out.values=QList<float>();

    auto l=resultKeys();
    for(int i=0;i<l.count();i++)
    {
        out.values.append(computeResult(l[i]));
    }

    return out;

}

QList<Result> Tent::results()
{
    QList<Result> output;
    QFile file(RESULTS_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        return output;
    }


    QDataStream out(&file);

    while(!out.atEnd())
    {
        Result r;


        out>>r.t;

        int n=resultKeys().count();
        for(int i=0;i<n;i++)
        {
            float vt;
            out>>vt;
            r.values.append(vt);
        }
        output<<r;

    }
    file.close();

    return output;
}

QStringList Tent::resultKeys()
{
    QStringList out;

    out<<"Electricity";
    out<<"Water";

    return out;
}

float Tent::computeResult(QString key)
{
    if(key=="Water")
    {
        return m_pumps->totalInjected();
    }

    return -1;
}

float Tent::resultAt(QString key)
{
    int i=resultKeys().indexOf(key);

    if(i<0)
        return -1;

    return currentResult().values[i];
}

QList<RealTimeValue> Tent::resultsFor(QString key)
{

    QList<RealTimeValue> out;
    auto l=results();
    for(int i=0;i<l.count();i++)
    {


        int id=resultKeys().indexOf(key);
        if(id>=0)
        {
            RealTimeValue v;
            v.time=l[i].t;
            v.value=l[i].values[id];
            out<<v;
        }
    }
    return out;
}

void Tent::clearResults()
{
    QFile::remove(RESULTS_FILE);
}

void Tent::saveResultsCsv()
{

}
*/
QList<RealTimeValue> Tent::injection(int id)
{
    if(id==0)
        return m_pumps->injectedHitstoric();

    return m_chemichals->injection(id);
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
        if(!m_serial->forbiden().contains(ls[i].portName()))
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
    for(int i=0;i<m_devices.count();i++)
    {
        m_devices[i]->startRecording(false);
    }
}
