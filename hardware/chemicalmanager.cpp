#include "chemicalmanager.h"
#include "hardware/Pinout.h"

#define VOLUME_TANK_KEY "Cuve [L]"
#define DEFAULT_CUVE "50" //L
ChemicalManager::ChemicalManager(QObject *parent)
    : HardwareUnit{parent}
{

    m_name="Engrais";
    attachInjector(new TankInjector(CHEM_MIX_1_PIN,CHEM_PUMP_1_PIN,NO_PIN,CHEMICAL_1,this));
    attachInjector(new TankInjector(CHEM_MIX_2_PIN,CHEM_PUMP_2_PIN,NO_PIN,CHEMICAL_2,this));
    attachInjector(new TankInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,NO_PIN,CHEMICAL_3,this));
    attachInjector(new TankInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,NO_PIN,CHEMICAL_4,this));
    attachInjector(new TankInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,NO_PIN,CHEMICAL_5,this));
    attachInjector(new TankInjector(CHEM_MIX_3_PIN,CHEM_PUMP_3_PIN,NO_PIN,CHEMICAL_6,this));



    m_idParameters<<CHEMICAL_1<<CHEMICAL_2<<CHEMICAL_3<<CHEMICAL_4<<CHEMICAL_5<<CHEMICAL_6;


    setDescription("En charge de l'injection des engrais liquides (le PH est inclut dans sa propre unité de controle");

}

void ChemicalManager::attachInjector(TankInjector *c)
{
    attachDevice(c->pump());
    attachDevice(c->mixer());

    c->enable(false);
    attachDevice(c->levelSensor());
    m_injectors.append(c);
    connect(c,SIGNAL(injection(int)),this,SLOT(injectorSlot(int)));
    connect(c,SIGNAL(console(QString)),this,SLOT(injectorConsole(QString)));

    if(c->id()>0)
        m_idParameters<<c->id();
}

void ChemicalManager::reactToSensorsChanged()
{

}

void ChemicalManager::reactToParamChanged(Parameter *p, float v)
{
    for(int i=0;i<m_injectors.count();i++)
            if(m_injectors[i]->id()==p->id())
            {
                m_injectors[i]->setCurrentValue(v);
            }
}

void ChemicalManager::attachParameter(Parameter *p)
{
    HardwareUnit::attachParameter(p);

    if(!p)
        return;


    for(int i=0;i<m_injectors.count();i++)
    {
        if(m_injectors[i]->id()==p->id())
        {
            m_injectors[i]->setName(p->name());
            m_injectors[i]->enable(true);
            return;
        }
    }
}



void ChemicalManager::fillTank()
{

    qDebug()<<"FillTank manajge";
    for(int i=0;i<m_injectors.count();i++)
    {
       m_injectors[i]->fillTank();
    }
}

ChemicalInjector::State ChemicalManager::currentState()
{

    ChemicalInjector::State sw=ChemicalInjector::ready;
    for(int i=0;i<m_injectors.count();i++)
    {

        auto s=m_injectors[i]->state();
        if(s>sw)
            sw=s;

    }

   return sw;
}

void ChemicalManager::injectorSlot(int )
{

    emit injectingStateChanged(currentState());
}

QList<ChemicalInjector *> ChemicalManager::injectors() const
{
    QList<ChemicalInjector *>  out;

    for(int i=0;i<m_injectors.count();i++)
        out<<m_injectors[i];

    return out;
}

QList<RealTimeValue> ChemicalManager::injection(int id)
{

    for(int i=0;i<m_injectors.count();i++)
    {
        if(m_injectors[i]->id()==id)
            return m_injectors[i]->injectionHistoric();
    }


    return QList<RealTimeValue>();

}

QList<Actuator *> ChemicalManager::interestingIntegrals()
{
    QList<Actuator *> out;
    for(int i=0;i<m_injectors.count();i++)
    {
        if(m_injectors[i]->isEnabled())
            out<<m_injectors[i]->pump();
    }
    return out;
}

void ChemicalManager::fillTankSlot(bool l)
{
    if(l)
        fillTank();
}

TankInjector::TankInjector(int mixPin, int pumpPin, int LevelPin ,int ID, QObject*parent):
    ChemicalInjector(mixPin,pumpPin,LevelPin,ID,parent), m_current(0)
{
    pump()->setDataValue(VOLUME_TANK_KEY,DEFAULT_CUVE);
}



void TankInjector::setCurrentValue(float v)
{


    m_current=v;

}

float TankInjector::volumeTank()
{
    return pump()->dataValue(VOLUME_TANK_KEY).toFloat();
}

float TankInjector::injectingVolumeMl()
{
    return m_current*volumeTank();
}

void TankInjector::fillTank()
{
    if(isEnabled())
        injectMl(injectingVolumeMl());
}

