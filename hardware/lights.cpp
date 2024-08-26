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
    m_switchTimer=new QTimer(this);
    connect(m_switchTimer,SIGNAL(timeout()),this,SLOT(switchLights()));

    attachDevice(m_power=new Lights(LIGHTS_POWER_PIN,this));
    attachDevice(m_spectrum=new LightsSpectrum(LIGHTS_SPECTRUM_PIN,this));

    m_idParameters<<LIGHTS_DAY<<LIGHTS_SLEEP<<LIGHTS_SPECTRUM;
}

void LightsUnit::begin()
{
    HardwareUnit::begin();



    int t=nextSwitchms();
    m_switchTimer->setInterval(t);
    m_switchTimer->start();
}

void LightsUnit::switchLights()
{
    if(m_power->currentValue()>50)
    {
        m_power->applyValue(0);
    }
    else
        m_power->applyValue(100);

    int t=nextSwitchms();


    m_switchTimer->setInterval(t);
    m_switchTimer->start();

}

void LightsUnit::reactToParamChanged(Parameter *p, float f)
{
    if(p==spectrum())
    {
        updateSpectrum(f);
        return;
    }
}

int LightsUnit::nextSwitchms()
{

    int h=m_startTime.secsTo(QDateTime::currentDateTime());
    auto a=night();
    auto b=day();
    int ht=0;
    //qDebug()<<"Cannot compute next switch "<<h;
    for(int i=0;i<b->count();i++)
    {
        float j=b->at(i).y();
        float n=a->at(i).y();
         //qDebug()<<"made rev"<<i<<j<<n<<ht<<h;

        ht+=j;
        if(ht>h)
        {
            return j*1000;
        }


        ht+=n;
        if(ht>h)
        {
             return n*1000;
        }


    }

  //  qDebug()<<"Cannot compute next switch ";
    return 500;

}

void LightsUnit::updateSpectrum(float t)
{
   // qDebug()<<"updateSpectrum"<<t;
    m_spectrum->applyValue(t);
}


