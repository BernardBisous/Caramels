#ifndef WATERLEVELMANAGER_H
#define WATERLEVELMANAGER_H

#include <QObject>
#include "hardware/booleansensor.h"
#include "hardware/chemicalinjector.h"
#include "hardware/pump.h"
#include "hardwareunit.h"

class WaterLevelManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit WaterLevelManager(QObject *parent = nullptr);
    void attachInjector(ChemicalInjector* c);
    virtual void reactToSensorsChanged();
    void fillTank();


    QString injectingState() const;
    bool injecting();

private slots:
    void injectorSlot(int i);
    void pumpSlot();
    void injectorConsole(QString s){console(s);}




signals:
    void injecting(bool t);
    void injectingStateChanged(QString s);

private:
    Pump* m_pump;
    BooleanSensor* m_levelDown;
    BooleanSensor* m_levelUp;
    SwitchedActuator* m_entryValve;
    QList<ChemicalInjector*> m_injectors;
    QString m_injectingState;



};

#endif // WATERLEVELMANAGER_H
