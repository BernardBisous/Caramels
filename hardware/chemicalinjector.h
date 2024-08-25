#ifndef CHEMICALINJECTOR_H
#define CHEMICALINJECTOR_H

#include "hardware/booleansensor.h"
#include "hardware/hardwareunit.h"
#include "hardware/pump.h"
#include <QObject>

class ChemicalInjector : public HardwareUnit
{
    Q_OBJECT
public:
    explicit ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int id,QObject *parent = nullptr);
    ChemicalPump *pump() const;
    void setPump(ChemicalPump *newPump);


signals:

private:
    ChemicalPump* m_pump;
    SwitchedActuator* m_mixer;
    BooleanSensor* m_levelSensor;



};

#endif // CHEMICALINJECTOR_H
