#include "lights.h"
#include "hardware/Pinout.h"



Lights::Lights(int pin, QObject* parent):SwitchedActuator(pin,false,"Lighs 400W_XE",parent)
{

    setDataValue("Gain [m2/W]","10");
    setResult("Power[W]");

}

float Lights::computeResult(QString s)
{
    if(s=="Power")
        return 2;
    return 1;
}

LightsSpectrum::LightsSpectrum(int pwmPin, QObject *parent):
    Actuator("Spectrum Potentiometer",parent)
{

}

LightsUnit::LightsUnit(QObject *parent):HardwareUnit{parent}
{
    m_name="Lights";
    attachDevice(m_power=new Lights(LIGHTS_POWER_PIN,this));
    attachDevice(m_spectrum=new LightsSpectrum(LIGHTS_SPECTRUM_PIN,this));

    m_idParameters<<LIGHTS_DAY<<LIGHTS_SLEEP<<LIGHTS_SPECTRUM<<LIGHTS_HEIGH;
}

void LightsUnit::begin()
{
    HardwareUnit::begin();

    m_power->retreiveLastValue();
    QDateTime t=nextSwitch();

    int n=t.secsTo(t);// /3600 !!
    m_timer->setInterval(n);
    m_timer->start();

}

void LightsUnit::update()
{
    if(m_power->currentValue()>50)
    {
        m_power->applyValue(0);
    }
    else
        m_power->applyValue(100);

    QDateTime t=nextSwitch();

    int n=t.secsTo(t);// /3600!!
    m_timer->setInterval(500);
    m_timer->start();

    qDebug()<<"swited lights for the next hours"<<n<<m_power->currentValue();
}

QDateTime LightsUnit::nextSwitch()
{
    int h=m_startTime.secsTo(QDateTime::currentDateTime());
    auto a=night();
    auto b=day();
    int ht=0;
    qDebug()<<"Cannot compute next switch "<<h;
    for(int i=0;i<b->count();i++)
    {
        float j=b->at(i).y();
        float n=a->at(i).y();
         qDebug()<<"made rev"<<i<<j<<n<<ht<<h;

        ht+=j;
        if(ht>h)
        {
            return QDateTime::currentDateTime().addSecs(ht);
        }


        ht+=n;
        if(ht>h)
        {
            return QDateTime::currentDateTime().addSecs(ht);
        }


    }

    qDebug()<<"Cannot compute next switch ";
    return QDateTime::currentDateTime().addSecs(5);

}


