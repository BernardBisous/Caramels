#ifndef LIGHTS_H
#define LIGHTS_H


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

private:
    Lights* m_power;
    LightsSpectrum* m_spectrum;
};
#endif // LIGHTS_H
