#include "device.h"
//#include "hardware/rasppi.h"
#include "qdatetime.h"
#include "qdir.h"
#include <QFile>

#define HISTO_PATH "data/"
#define PR_SAMPLING_RATE "Freq.[s]"

#define AUTO_POLL_SENSOR false
#define MAX_VALUES 200
#define MAX_DELAY_RECORD 2
Device::Device(QString name,QObject *parent)
    : QObject{parent},m_recording(false),
      m_name(name),m_serial(nullptr),m_recordDelay(-1),m_lastRecord()
{
    setRange(0,100);
    setUnits("%");
}

void Device::begin()
{
    retreiveLastValue();
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

void Device::reactToDataEdited(QString key, QString val)
{
    if(key=="Gain")
    {
        m_gain=val.toFloat();
    }
    if(key=="Offset")
    {
        m_offset=val.toFloat();
    }
}

void Device::reset()
{
    startRecording(false);

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

QList<RealTimeValue> Device::historic(int size)
{
    QList<RealTimeValue> in=historic();
    QList<RealTimeValue> out=historic();
    int n=in.count();
    int i=n-size-1;
    if(i<0)
        i=0;
    for(;i<n;i++)
    {
        out<<in[i];
    }
    return out;
}

void Device::retreiveLastValue()
{
    m_values=historic(MAX_VALUES);
}

void Device::setRange(float min, float max)
{
    float g=(max-min);
    g=g/100;

    float o=min;

    setGain(g);
    setOffset(o);
}

void Device::setGain(float t)
{
    setDataValue("Gain",QString::number(t));
    m_gain=t;
}

void Device::setOffset(float t)
{
    setDataValue("Offset",QString::number(t));
    m_offset=t;
}

float Device::maxRange()
{
    return m_gain*100+m_offset;
}

float Device::minRange()
{
    return m_offset;
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

float Device::currentPurcent()
{
    return (currentValue()-m_offset)/m_gain;
}

void Device::storeValue(float t)
{
    int ts=m_lastRecord.secsTo(QDateTime::currentDateTime());

    if(m_recordDelay>0
            && m_lastRecord.isValid()
            && ts<m_recordDelay)

        return;

    if(name()=="Capteur de PH")
        qDebug()<<"store value"<<t<<ts<<m_lastRecord;

    m_lastRecord=QDateTime::currentDateTime();

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
    m_values.append({QDateTime::currentDateTime(),v});

    while(m_values.count()>MAX_VALUES)
        m_values.takeFirst();

    if(m_recording)
        storeValue(v);

    emit newValue(v);
}

void Device::setRecordDelay(int newRecordDelay)
{
    m_recordDelay = newRecordDelay;
}

float Device::gain() const
{
    return m_gain;
}

void Device::exportHistoric(QString dir)
{
    QString se=",";
    auto l=historic();

    qDebug()<<"export device"<<name()<<l.count();
    QFile f(dir+"/"+name()+".csv");
    f.open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream t(&f);
    t.setEncoding(QStringConverter::Utf8);
    QString h=name()+se+"Time"+"\n";
    t<<h;

    for(int i=0;i<l.count();i++)
    {
        QString sl=l[i].time.toString("yyyy-MM-dd HH:mm:ss");
        sl+=se;
        sl+=QString::number(l[i].value);
        sl+="\n";
        t<<sl;

    }

    f.close();
}

void Device::clearHistoric()
{
    QFile::remove(storageFile());
}

QList<RealTimeValue> Device::values() const
{
    return m_values;
}

QString Device::units() const
{
    return m_units;
}

void Device::setUnits(const QString &newUnits)
{
    m_units = newUnits;
}

SerialTent *Device::serial() const
{
    return m_serial;
}

void Device::setSerial(SerialTent *newSerial)
{
    m_serial = newSerial;
}

float Device::currentValue() const
{
    if(m_values.isEmpty())
        return -1;

    return m_values.last().value;
}


SwitchedActuator::SwitchedActuator(int pin, bool pwm, QString name, QObject *parent)
    :Actuator(name,parent),m_pin(pin),m_pwmAnalog(pwm)
{

}

void SwitchedActuator::applyPurcent(float v)
{

    if(m_pwmAnalog)
        m_serial->write(m_pin,v);

    else m_serial->write(m_pin,(v>0)*100);

    Actuator::applyPurcent(v);
}

float SwitchedActuator::filterInputValue(float v)
{
    if(m_pwmAnalog)
        return v;

    if(v>50)
        return 100;
    return 0;
}

Actuator::Actuator(QString name, QObject *parent):
    Device(name,parent)
{
    m_impulseTimer=new QTimer;
    connect(m_impulseTimer,SIGNAL(timeout()),this,SLOT(impulseSlot()));
}

void Actuator::applyPurcent(float v)
{

}

void Actuator::applyValue(float v)
{

   // qDebug()<<"Actuator apply value"<<m_name<<v<<(v-m_offset)/m_gain;
    applyPurcent((v-m_offset)/m_gain);
    appendValue(v);
}

void Actuator::userApplyPurcent(float v)
{
    float f=filterInputValue(v);
    applyPurcent(f);
    appendValue(f*m_gain+m_offset);
}

void Actuator::reset()
{
    stop();
    Device::reset();
}

void Actuator::stop()
{
    m_impulseTimer->stop();

    userApplyPurcent(0);
}

void Actuator::impulse(float val,int ms, float valEnd)
{
    applyValue(val);
    m_nextVal=valEnd;
    m_impulseTimer->start(ms);
}

void Actuator::impulseHigh(int ms)
{
    impulse(maxRange(),ms,minRange());
}

void Actuator::impulseSlot()
{

    m_impulseTimer->stop();
    applyValue(m_nextVal);
    emit impulseDone();

}

Sensor::Sensor(int pin,QString name, QObject *parent)
    :Device(name,parent),m_pollTimer(nullptr),m_pin(pin),m_continousStreaming(false)
{

    if(AUTO_POLL_SENSOR)
        setDataValue("Freq.[s]","1");

    setRecordDelay(MAX_DELAY_RECORD);
}

float Sensor::aquire()
{

    return m_serial->read(m_pin)*m_gain+m_offset;
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

    if(AUTO_POLL_SENSOR)
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

    if(m_continousStreaming || a!=currentValue())
    {
        appendValue(a);
    }

}



Motor::Motor(int dirpin, int pwm, QString name, QObject *parent):
    Actuator(name,parent),m_dirPin(dirpin),m_pwmPin(pwm)
{

}

void Motor::applyValue(float v)
{

}

