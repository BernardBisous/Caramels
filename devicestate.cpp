#include "devicestate.h"



DeviceState::DeviceState(Device *d, HardwareUnit *h, QObject *parent):
    QObject(parent),m_unit(h),m_device(d),m_criticity(Good)
{

    if(d)
        connect(d,SIGNAL(error(QString,bool)),this,SLOT(clientError(QString,bool)));

    if(h)
        connect(h,SIGNAL(error(QString,bool)),this,SLOT(clientError(QString,bool)));

}

void DeviceState::refresh()
{
    if(m_device)
    {
        bool s;
        QString d=m_device->diagnose(&s);
        clientError(d,s);
    }

    if(m_unit)
    {
        bool s;
        QString d=m_unit->diagnose(&s);
        clientError(d,s);
    }


}

void DeviceState::setState(QString d, Criticity s)
{
    if(d==m_diagnosis && s==m_criticity)
        return;

    m_criticity=s;
    m_diagnosis=d;



    emit changed();
}

QString DeviceState::fullDiagnosis()
{
    QString out;

    if(m_device)
        out+=m_device->name()+" : ";

    if(m_unit)
        out+=m_unit->name()+" : ";

    out+=diagnosis();
    return name()+" : "+diagnosis();
}

void DeviceState::clientError(QString s, bool w)
{
    m_diagnosis=s;
    if(s.isEmpty())
        setState(s,Good);

    else if(w)
        setState(s,Warning);

    else
        setState(s,Danger);
}

QString DeviceState::diagnosis() const
{
    return m_diagnosis;
}

QString DeviceState::name()
{
    if(m_unit)
        return m_unit->name();

    if(m_device)
        return m_device->name();

    return "";
}

DeviceState::Criticity DeviceState::criticity() const
{
    return m_criticity;
}

Device *DeviceState::device() const
{
    return m_device;
}

HardwareUnit *DeviceState::unit() const
{
    return m_unit;
}



SerialState::SerialState(SerialTent *c, QObject *parent):
    DeviceState(nullptr,nullptr,parent),m_client(c)
{
    connect(c,SIGNAL(connectedChanged(bool )),this,SLOT(changedState(bool)));
    if(!c->isConnected())
        clientError("Pas d'électronique trouvée",false);
}

QString SerialState::diagnosis() const
{
    if(m_client->isConnected())
        return "";

    return "Pas d'électronique trouvée";
}

QString SerialState::name()
{
    return "Carte de controle";
}

void SerialState::refresh()
{
    clientError(diagnosis(),false);
}

void SerialState::changedState(bool s)
{
    refresh();
}

