#ifndef WATERLEVELMANAGER_H
#define WATERLEVELMANAGER_H

#include <QObject>
#include "hardware/booleansensor.h"
#include "hardware/chemicalinjector.h"
#include "hardware/pump.h"
#include "hardwareunit.h"

class TankInjector: public ChemicalInjector
{
    Q_OBJECT
 public:
    explicit TankInjector(int mixPin, int pumpPin, int LevelPin, int ID, QObject *parent = nullptr);

    void setCurrentValue(float v);
    void fillTank();

private:
    float m_current;
};

class WaterLevelManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit WaterLevelManager(QObject *parent = nullptr);
    void attachInjector(TankInjector *c);
    virtual void reactToSensorsChanged();
    virtual void reactToParamChanged(Parameter*, float );

    virtual void attachParameter(Parameter* p);
    void fillTank();


    QString injectingState() const;
    bool injecting();

private slots:
    void injectorSlot(int i);

    void injectorConsole(QString s){console(s);}




signals:

    void injectingStateChanged(QString s);

private:
    Pump* m_pump;
    BooleanSensor* m_levelDown;
    BooleanSensor* m_levelUp;
    SwitchedActuator* m_entryValve;
    QList<TankInjector*> m_injectors;
    QString m_injectingState;



};

#endif // WATERLEVELMANAGER_H
