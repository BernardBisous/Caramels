#include "device.h"
//#include "hardware/rasppi.h"
#include "qdatetime.h"
#include "qdir.h"
#include "qsettings.h"
#include <QFile>

#define HISTO_PATH "data/"
#define MAX_VALUES 2
#define MAX_KEY "Max"
#define MIN_KEY "Min"
#define INTEGRAL_KEY "Total"

#define LOAD_META true


Device::Device(QString name,QObject *parent)
    : QObject{parent},m_maxValues(MAX_VALUES),
      m_recording(true),m_name(name),
      m_serial(nullptr),m_lastRecord(),m_recordDelay(-1),
      m_continousStreaming(false),m_parameter(nullptr),m_enable(true)
{
    setRange(0,1);
    setUnits("");
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

void Device::reactToDataEdited(QString, QString)
{
     computeGains();
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
        float v=computeResult(l[i]);
        m_metaResults.insert(l[i],v);

    }
}

QString Device::dataValue(QString key) const
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
    {
        QSettings settings("YourOrganization", name());
        settings.setValue(key,val);
        reactToDataEdited(key,val);
    }
}

void Device::loadSettings()
{
    if(!LOAD_META)
        return;
    QSettings settings("YourOrganization", name());

    for(QHash<QString, QString>::iterator i = m_metaData.begin(); i !=m_metaData.end(); ++i)
    {
        if(settings.contains(i.key()))
            setDataValue(i.key(),settings.value(i.key()).toString());
    }


    computeGains();
}

void Device::setResult(QString key, QString units)
{
    m_metaResults.insert(key,0);
    m_resultsUnits.insert(key,units);
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

    if(!output.isEmpty())
        output.takeLast();

    return output;
}

QList<RealTimeValue> Device::historic(int size)
{
    QList<RealTimeValue> in=historic();
    QList<RealTimeValue> out;

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
    m_values=historic(m_maxValues);
}

QString Device::userValue()
{

    return QString::number(currentValue(),'f',1)+" "+units();
}

float Device::neutralValue()
{
    if(m_gain==0)
        return 0;

    return neutralPurcent()/m_gain-m_offset;
}

void Device::setRange(float min, float max)
{
    setDataValue(MAX_KEY,QString::number(max));
    setDataValue(MIN_KEY,QString::number(min));
    computeGains();
}

void Device::setMaxRange(float max)
{
    setDataValue(MAX_KEY,QString::number(max));
     computeGains();
}

float Device::mapToPurcent(float val)
{
    return (val-m_offset/gain());
}

float Device::maxRange()
{
    return dataValue(MAX_KEY).toFloat();
}

float Device::minRange()
{
    return dataValue(MIN_KEY).toFloat();
}

void Device::computeGains()
{   
    float min=minRange();
    float g=(maxRange()-min);
    m_gain=g/100;

    m_offset=min;
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

void Device::storeValue(RealTimeValue t)
{

    m_lastRecord=t.time;
    QFile file(storageFile());
    if (!file.open(QIODevice::Append)) {
        return;
    }

    QDataStream out(&file);
    out << t.time;
    out << t.value;

    file.close();
    return;
}

void Device::appendValue(float v)
{
    if(!m_continousStreaming &&  v==currentValue())
        return;


    QDateTime now=QDateTime::currentDateTime();

    RealTimeValue rt{now,v};
    m_values.append(rt);

    while(m_values.count()>m_maxValues)
        m_values.takeFirst();

    if(m_recording)
    {

        bool store= m_lastRecord.isValid() && m_recordDelay>0 && m_lastRecord.secsTo(now)>=m_recordDelay;
        store=store || m_recordDelay<0 || !m_lastRecord.isValid();


       if(store)
            storeValue(rt);
    }

    computeResults();

    emit newValue(v);
}

int Device::maxValues() const
{
    return m_maxValues;
}

bool Device::enabled() const
{
    return m_enable;
}

void Device::enable(bool newEnable)
{
    m_enable=newEnable;
}



QHash<QString, QString> Device::resultsUnits() const
{
    return m_resultsUnits;
}

void Device::setResultUnits(QString key, QString units)
{
    m_resultsUnits.insert(key,units);
}

QHash<QString, float> Device::metaResults() const
{
    return m_metaResults;
}

Parameter *Device::parameter() const
{
    return m_parameter;
}

bool Device::continousStreaming() const
{
    return m_continousStreaming;
}

void Device::attachParameter(Parameter *p)
{
    m_parameter=p;
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



float SwitchedActuator::filterInputValue(float v)
{

    if(m_pwmAnalog)
        return v;

    if(v>50)
        return 100;

    return 0;
}

void SwitchedActuator::applyPurcent(float v)
{
    if(m_serial)

        m_serial->write(m_pin,v);
}

Actuator::Actuator(QString name, QObject *parent):
    Device(name,parent),m_integratedInteresting(true),m_integral(0)
{
    setResult(INTEGRAL_KEY); // migh be an issues to call virtual functions in contructor
    m_impulseTimer=new QTimer(this);
    connect(m_impulseTimer,SIGNAL(timeout()),this,SLOT(impulseSlot()));
}

void Actuator::applyPurcent(float )
{

}

void Actuator::retreiveLastValue()
{
    Device::retreiveLastValue();
    computeHistoricIntegral();
}

QString Actuator::userValue()
{
    if(currentValue()==0)
        return "Off";
    return Device::userValue();
}

void Actuator::applyIntegral(float v)
{
    float f=v-integral();
    float t=f/m_gain;

    impulseHigh(t*1000);
}

void Actuator::switchStateUser()
{
    setStateHigh(currentPurcent()==neutralPurcent());
}

void Actuator::setStateHigh(bool s)
{
    if(s)
        userApplyPurcent(100);

    else
        userApplyPurcent(neutralPurcent());
}

void Actuator::applyValue(float v)
{
    if(!m_values.isEmpty())
    {
        m_integral=integral();

    }

    applyPurcent((v-m_offset)/m_gain);
    appendValue(v);

   // qDebug()<<"apply value"<<name()<<v<<m_integral<<m_metaResults;
}

void Actuator::userApplyPurcent(float v)
{
   applyValue(filterInputValue(v)*m_gain+m_offset);
}

void Actuator::reset()
{
    stop();
    m_integral=0;
    Device::reset();
}

void Actuator::stop()
{
    m_impulseTimer->stop();
    userApplyPurcent(neutralPurcent());
}

void Actuator::impulse(float val,int ms, float valEnd)
{
    applyValue(val);
    m_nextVal=valEnd;
    m_impulseTimer->start(ms);
}

void Actuator::impulseHigh(int ms)
{
    impulse(maxRange(),ms,neutralPurcent());
}

void Actuator::setIntegralUnits(QString s)
{
    setResultUnits(INTEGRAL_KEY,s);
}

float Actuator::computeResult(QString s)
{
    if(s==INTEGRAL_KEY)
        return m_integral;

    return Device::computeResult(s);
}

void Actuator::computeHistoricIntegral()
{
    auto l=integralHistoric();

    if(!l.isEmpty())
        m_integral=integralHistoric().last().value;
}

QList<RealTimeValue> Actuator::integralHistoric()
{

    QList<RealTimeValue> l=historic();
    QList<RealTimeValue> out;
    float v=0;
    for(int i=0;i<l.count();i++)
    {
        if(i)
        {
            RealTimeValue t;
            t.time=l[i].time;

            int dx=l[i-1].time.msecsTo(t.time);
            v+=(dx*l[i-1].value)/1000;

            t.value=v;

            out<<t;
        }

    }

    return out;
}

void Actuator::impulseSlot()
{



    m_impulseTimer->stop();
    applyValue(m_nextVal);
    emit impulseDone();

}

bool Actuator::integratedInteresting() const
{
    return m_integratedInteresting;
}

void Actuator::setIntegratedInteresting(bool newIntegratedInteresting)
{
    m_integratedInteresting = newIntegratedInteresting;
}

float Actuator::integral() const
{
    return immediateIntegral();
}

float Actuator::immediateIntegral() const
{


    //TO fix
    if(m_values.isEmpty())
        return 0;

    float s=m_values.last().time.msecsTo(QDateTime::currentDateTime())*currentValue();

    return s/1000+m_integral;
}

QString Actuator::integralUnit()
{
    return m_resultsUnits.value(INTEGRAL_KEY);
}

Sensor::Sensor(int pin,QString name, QObject *parent)
    :Device(name,parent),m_pin(pin)
{



}

float Sensor::aquire()
{

   // qDebug()<<"acquiered"<<name()<<m_serial->read(m_pin);
    return m_serial->read(m_pin)*m_gain+m_offset;
}



void Sensor::begin()
{
    Device::begin();


}



void Sensor::measure()
{
    float a=aquire();

    appendValue(a);
}



Motor::Motor(int p1, int p2, QString name, QObject *parent):
    Actuator(name,parent),m_pin1(p1),m_pin2(p2)
{

}

void Motor::applyValue(float )
{

}

void Motor::applyPurcent(float v)
{
    if(v==50)
    {
        m_serial->write(m_pin1,0);
        m_serial->write(m_pin2,0);
    }
    else if(v<50)
    {
        if(m_clockwise)

            m_serial->write(m_pin2,0);

        else
            m_serial->write(m_pin1,v+50);

        m_clockwise=false;
    }
    else
    {
        if(!m_clockwise)
            m_serial->write(m_pin1,0);

        else
            m_serial->write(m_pin2,v+50);

        m_clockwise=true;
    }



}



void Motor::setMaxSpeed(float v)
{
    setRange(-v,v);
}

float Motor::maxSpeed()
{
    return maxRange();
}

void Motor::move(float t)
{

    qDebug()<<"motor moving to "<<t;
    if(t==0)
        return;

    if(t<0)
    {
        float s=-t/maxSpeed();
        impulse(-maxSpeed(),s*1000,0);
    }
    else
    {
        float s=t/maxSpeed();
        impulse(maxSpeed(),s*1000,0);
    }
}

void Motor::moveTo(float t)
{
    move(t-position());
}

float Motor::position()
{
    return m_integral;
}

