#ifndef WATERLEVELMANAGER_H
#define WATERLEVELMANAGER_H

#include <QObject>
#include "hardware/Pinout.h"
#include "hardware/booleansensor.h"
#include "hardware/chemicalinjector.h"
#include "hardware/pump.h"
#include "hardwareunit.h"



class WaterLevelManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit WaterLevelManager(QObject *parent = nullptr);

    virtual QList<Device*> interestingDevices();
    virtual QList<Actuator*> interestingIntegrals();
    virtual void reactToSensorsChanged();
    virtual void reactToParamChanged(Parameter*, float );

    Parameter* dry(){return parameterFromId(DRY_DELAY);}
    Parameter* wet(){return parameterFromId(WET_DELAY);}

    void fillTank();
    bool filling() const;
    bool isDry();

    float totalInjected();
    QList<RealTimeValue> injectedHitstoric();


    Pump *entryValve() const;

    SwitchedActuator *pump() const;

signals:
    void fillingTank(bool s);

private slots:
    void switchDry();

private:
    SwitchedActuator* m_pump;
    BooleanSensor* m_levelDown;
    BooleanSensor* m_levelUp;
    Pump* m_entryValve;
    bool m_filling;


};

#endif // WATERLEVELMANAGER_H
