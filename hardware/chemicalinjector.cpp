#include "chemicalinjector.h"

ChemicalInjector::ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int id,QObject *parent )
    :HardwareUnit{parent}
{
    attachDevice(m_pump=new ChemicalPump(pumpPin,"Pompe engrais "+QString::number(id),this));
    attachDevice(m_mixer=new SwitchedActuator(mixPin,true,"Brasseur engrais "+QString::number(id),this));
    attachDevice(m_levelSensor=new BooleanSensor(LevelPin,"Niveau engrais "+QString::number(id),this));

    m_idParameter=id;
}


ChemicalPump *ChemicalInjector::pump() const
{
    return m_pump;
}

void ChemicalInjector::setPump(ChemicalPump *newPump)
{
    m_pump = newPump;
}

