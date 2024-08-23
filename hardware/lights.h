#ifndef LIGHTS_H
#define LIGHTS_H


#include "hardware/device.h"

class Lights: public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Lights(int pint,QObject *parent = nullptr);
};

class LightsSpectrum: public Actuator
{
    Q_OBJECT
public:
    explicit LightsSpectrum(QObject *parent = nullptr);
};

#endif // LIGHTS_H
