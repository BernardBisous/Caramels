#include "regulatingtimer.h"

RegulatingTimer::RegulatingTimer(QObject *parent)
    : QObject{parent},m_lastSwitch(),
      m_high(0),m_low(0),m_highString("ON"),m_lowString("OFF"),
      m_hoursRoutine(1),m_startsHigh(true)
{
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    m_timer->setInterval(1000*3600);
}

void RegulatingTimer::setDelaysHours(float delayHigh, float delayLow)
{
    setDelayHigh(delayHigh);
    setDelayLow(delayLow);
}

void RegulatingTimer::reset()
{
    m_timer->stop();
    m_high=0;
    m_low=0;
    m_lastSwitch=QDateTime();
}

void RegulatingTimer::startState(bool high)
{

    m_lastSwitch=QDateTime::currentDateTime();
    m_state=high;

    if(m_device)
    {
        m_device->setStateHigh(m_state);
    }

    continueTimer();
}

bool RegulatingTimer::shouldChange()
{
    if(!m_lastSwitch.isValid())
        return true;

    return nextSwitch()>QDateTime::currentDateTime();
}

void RegulatingTimer::setDelayHigh(float h)
{
    m_high=h;




    if(m_high && m_low && !m_timer->isActive())
        startState(m_startsHigh);
}

void RegulatingTimer::setDelayLow(float h)
{
    m_low=h;

   if(m_high && m_low && !m_timer->isActive())
        startState(m_startsHigh);
}

QDateTime RegulatingTimer::nextSwitch()
{
    QDateTime out=m_lastSwitch;
    if(!out.isValid())
        return out;

    if(m_state)
        return out.addSecs(m_high*3600);
    else
        return out.addSecs(m_low*3600);
}

float RegulatingTimer::remainingHours()
{
    if(!m_lastSwitch.isValid())
        return 0;

    int s=QDateTime::currentDateTime().secsTo(nextSwitch());
   // s=s-1;// Rounding margin to 1s

    if(s<=0)
        return 0;

    float v=s;
    v=v/3600;
    return v;
}

void RegulatingTimer::timerSlot()
{

    if(remainingHours()<=0)
    {

        emit shouldSwitch(m_state);
        startState(!m_state);
        return;
    }

    continueTimer();
}

float RegulatingTimer::low() const
{
    return m_low;
}

void RegulatingTimer::continueTimer()
{

    float h=remainingHours();

    if(h==0)
    {
        m_timer->stop();
        return;
    }

    if(h>m_hoursRoutine)
        m_timer->start(m_hoursRoutine*3600000);

    else
        m_timer->start(h*3600000);
}

float RegulatingTimer::high() const
{
    return m_high;
}


Actuator *RegulatingTimer::device() const
{
    return m_device;
}

void RegulatingTimer::setDevice(Actuator *newDevice)
{
    m_device = newDevice;
}

bool RegulatingTimer::state() const
{
    return m_state;
}

QString RegulatingTimer::lowString() const
{
    return m_lowString;
}

void RegulatingTimer::setLowString(const QString &newLowString)
{
    m_lowString = newLowString;
}

void RegulatingTimer::setStateStrings(QString h, QString low)
{
    setLowString(low);
    setHighString(h);
}

QString RegulatingTimer::currentString()
{
    bool h=true;
    if(m_device && m_device->currentPurcent()==m_device->neutralPurcent())
        h=false;

    if(h)
        return m_highString;

    return m_lowString;
}

void RegulatingTimer::userSwitch()
{
    if(!m_device)
        return;

    m_device->switchStateUser();
}

QString RegulatingTimer::highString() const
{
    return m_highString;
}

void RegulatingTimer::setHighString(const QString &newHighString)
{
    m_highString = newHighString;
}
