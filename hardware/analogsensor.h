#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#include "hardware/device.h"
#include <QObject>

class AnalogSensor : public Sensor
{
    Q_OBJECT
public:
    explicit AnalogSensor(int pin,float gain,QString n,QObject *parent = nullptr);
    virtual float aquire();
    virtual void reactToDataEdited(QString key,QString val);


signals:

protected:
    int m_pin;
    float m_gain;

};

#endif // ANALOGSENSOR_H
