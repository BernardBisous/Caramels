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

signals:

private:
    Pump* m_pump;
    BooleanSensor* m_levelDown;
    BooleanSensor* m_levelUp;
    SwitchedActuator* m_entryValve;
    QList<ChemicalInjector*> m_injectors;
};

#endif // WATERLEVELMANAGER_H
