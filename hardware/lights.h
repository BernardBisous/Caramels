#ifndef LIGHTS_H
#define LIGHTS_H


#include "hardware/Pinout.h"
#include "hardware/device.h"
#include "hardware/hardwareunit.h"

class Lights: public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Lights(int pint,QObject *parent = nullptr);

    virtual float computeResult(QString s);
};

class LightsSpectrum: public Actuator
{
    Q_OBJECT
public:
    explicit LightsSpectrum(int pwmPin,QObject *parent = nullptr);
};
class LightsUnit: public HardwareUnit
{
    Q_OBJECT
public:
    explicit LightsUnit(QObject *parent = nullptr);
    virtual void begin();
    virtual void update();
    QDateTime nextSwitch();

    Parameter* night(){return parameterFromId(LIGHTS_SLEEP);}
    Parameter* day(){return parameterFromId(LIGHTS_DAY);}
    Parameter* spectrum(){return parameterFromId(LIGHTS_SPECTRUM);}
    Parameter* heigh(){return parameterFromId(LIGHTS_HEIGH);}

private:

    Lights* m_power;
    LightsSpectrum* m_spectrum;
};
#endif // LIGHTS_H
