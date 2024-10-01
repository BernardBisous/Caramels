#ifndef LIGHTS_H
#define LIGHTS_H


#include "hardware/Pinout.h"
#include "hardware/device.h"
#include "hardware/hardwareunit.h"

class Lights: public SwitchedActuator
{
    Q_OBJECT
public:
    explicit Lights(int pinSwitch,int pinPot,QObject *parent = nullptr);
    virtual float computeResult(QString s);
    virtual void userApplyPurcent(float);
    SwitchedActuator *relay() const;

private:
    SwitchedActuator* m_relay;
    float powerValue;
};

class LightsSpectrum: public SwitchedActuator
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
    virtual void reactToParamChanged(Parameter*, float );
    virtual void finish();
    bool isDayLight();

    virtual QList<Device*> interestingDevices();

    virtual QList<Actuator*> interestingIntegrals();

    void updateSpectrum(float);

    float lightPower();
    float spectrumValue();

    Parameter* night(){return parameterFromId(LIGHTS_SLEEP);}
    Parameter* day(){return parameterFromId(LIGHTS_DAY);}
    Parameter* spectrum(){return parameterFromId(LIGHTS_SPECTRUM);}
    Parameter* power(){return parameterFromId(LIGHTS_POWER);}

public slots:
    void regulatorSlot(bool s);

private:
    Lights* m_power;
    LightsSpectrum* m_spectrum;
    SwitchedActuator *m_switch;
};
#endif // LIGHTS_H
