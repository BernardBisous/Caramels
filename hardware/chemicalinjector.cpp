#include "chemicalinjector.h"
#include "hardware/Pinout.h"

ChemicalInjector::ChemicalInjector(int mixPin, int pumpPin, int LevelPin , int id, QObject *parent )
    :HardwareUnit{parent}
{
    m_name="Chemical injector "+QString::number(id);
    attachDevice(m_pump=new ChemicalPump(pumpPin,"Pompe engrais "+QString::number(id),this));
    attachDevice(m_mixer=new SwitchedActuator(mixPin,true,"Brasseur engrais "+QString::number(id),this));

    if(LevelPin)
        attachDevice(m_levelSensor=new BooleanSensor(LevelPin,"Niveau engrais "+QString::number(id),this));

    m_idParameters<<id+CHEMICAL_1;
}


ChemicalPump *ChemicalInjector::pump() const
{
    return m_pump;
}

void ChemicalInjector::attachParameter(Parameter *p)
{
    HardwareUnit::attachParameter(p);
    m_name=p->name();
}

void ChemicalInjector::setPump(ChemicalPump *newPump)
{
    m_pump = newPump;
}

