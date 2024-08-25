#include "chemicalinjector.h"

ChemicalInjector::ChemicalInjector(int id,QObject *parent)
    :HardwareUnit{parent}
{
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

