#ifndef CHEMICALMANAGER_H
#define CHEMICALMANAGER_H

#include "hardware/chemicalinjector.h"
#include "hardware/hardwareunit.h"
#include <QObject>

class TankInjector: public ChemicalInjector
{
    Q_OBJECT
 public:
    explicit TankInjector(int mixPin, int pumpPin, int LevelPin, int ID, QObject *parent = nullptr);

    void setCurrentValue(float v);
    float volumeTank();
    float injectingVolumeMl();
    void fillTank();

private:
    float m_current;
};
class ChemicalManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit ChemicalManager(QObject *parent = nullptr);
    void attachInjector(TankInjector *c);
    virtual void reactToSensorsChanged();
    virtual void reactToParamChanged(Parameter*, float );
    virtual void attachParameter(Parameter* p);
    void fillTank();
    ChemicalInjector::State currentState();
    QList<ChemicalInjector *> injectors() const;

    QList<RealTimeValue> injection(int id);
virtual QList<Actuator*> interestingIntegrals();
public slots:
    void fillTankSlot(bool l);
private slots:
    void injectorSlot(int i);
    void injectorConsole(QString s){console(s);}

signals:
    void injectingStateChanged(ChemicalInjector::State s);

private:
    QString m_injectingState;
    QList<TankInjector*> m_injectors;


};

#endif // CHEMICALMANAGER_H
