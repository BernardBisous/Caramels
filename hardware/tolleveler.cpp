#include "tolleveler.h"

TolLeveler::TolLeveler(QObject *parent)
    : HardwareUnit{parent}
{
    m_name="Hauteur des lights";
    attachDevice(m_lifter=new TopLifter(HEIGH_LIFTER_DIR,HEIGH_LIFTER_PWM,"Ascenseur de lights ",this));
    attachDevice(m_sensor=new DistanceSensor(HEIGH_TRIG_PRIN,HEIGH_ECHO_PIN,"Capteur de hauteur",this));


    connect(m_sensor,SIGNAL(newValue(float)),this,SLOT(heighNewValue(float)));
    m_idParameters<<LIGHT_HEIGH;
}

void TolLeveler::begin()
{
    HardwareUnit::begin();
}

void TolLeveler::reactToParamChanged(Parameter *p, float v)
{
    if(p==heigh())
    {
        m_distance=v;

    }
}

void TolLeveler::heighNewValue(float v)
{
    //qDebug()<<"new heigh value"<<v<<m_distance;
    float dist=m_distance-v;
    m_lifter->liftCm(dist);
}



TopLifter::TopLifter(int dir, int pwm, QString name, QObject *parent):
    Motor(dir,pwm,name,parent)
{
    setDataValue("Speed[cm/s]","1");
}

void TopLifter::liftCm(float t)
{

    float gain=dataValue("Speed[cm/s]").toFloat();
    int ms=qAbs(t)/gain*1000;
    float val=100;
    if(t<0)
        val=-val;

   // qDebug()<<"sent to rotation for"<<ms;
    applyValue(val,ms);
}
