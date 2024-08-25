#ifndef WATERLEVELMANAGER_H
#define WATERLEVELMANAGER_H

#include "hardware/booleansensor.h"
#include "hardware/hardwareunit.h"
#include "hardware/pump.h"
#include <QObject>

class WaterLevelManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit WaterLevelManager(QObject *parent = nullptr);

signals:

private:
    Pump* m_pump;
    BooleanSensor* m_levelDown;
    BooleanSensor* m_levelUp;
    SwitchedActuator* m_entryValve;
};

#endif // WATERLEVELMANAGER_H
