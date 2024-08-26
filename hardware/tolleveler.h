#ifndef TOLLEVELER_H
#define TOLLEVELER_H

#include "hardware/distancesensor.h"
#include "hardware/hardwareunit.h"
#include "hardware/Pinout.h"
#include <QObject>

class TopLifter : public Motor
{
    Q_OBJECT
public:
    explicit TopLifter(int dir, int pwm,QString name,QObject *parent = nullptr);
    void liftCm(float t);
};

class TolLeveler : public HardwareUnit
{
    Q_OBJECT
public:
    explicit TolLeveler(QObject *parent = nullptr);
    virtual void begin();
    virtual void reactToParamChanged(Parameter*, float );

    Parameter* heigh(){return parameterFromId(LIGHT_HEIGH);}

private slots:
    void heighNewValue(float v);

signals:

private:
    float m_distance;
    TopLifter* m_lifter;
    DistanceSensor* m_sensor;

};

#endif // TOLLEVELER_H
