#include "hardwareunit.h"

HardwareUnit::HardwareUnit(QObject *parent)
    : QObject{parent}
{

}

void HardwareUnit::attachDevice(Device *d)
{
    if(devFromName(d->name()))
        renameDev(d);


     m_devices.append(d);
}

void HardwareUnit::attachParameter(Parameter *p)
{
    m_parameters.append(p);
}

int HardwareUnit::parameterPossibleId()
{
    return m_idParameter;
}

Device *HardwareUnit::devFromName(QString s)
{
    for(int i=0;i<m_devices.count();i++)
    {
        if(m_devices[i]->name()==s)
            return m_devices[i];
    }
    return nullptr;

}

void HardwareUnit::renameDev(Device *d)
{
    QString nouv=d->name();
    // remove index
    while(!nouv.isEmpty() && nouv.at(nouv.length()-1).isDigit())
    {
        nouv.remove(nouv.length()-1,1);
    }

    // find new index
    int index=0;
    while(devFromName(nouv+QString::number(index+1)))
    {
        index++;
    }
    d->setName(nouv+QString::number(index+1));
}

QList<Device *> HardwareUnit::devices() const
{
    return m_devices;
}

QList<Parameter *> HardwareUnit::parameters() const
{
    return m_parameters;
}

QString HardwareUnit::name() const
{
    return m_name;
}
