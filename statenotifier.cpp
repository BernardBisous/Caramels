#include "statenotifier.h"
#include "Interface/emailnotifier.h"

#define SEND_MAILS true
#define EMAIL_DELAY_MS 1000
StateNotifier::StateNotifier(QObject *parent)
    : QObject{parent},m_criticity(DeviceState::Good)
{
    m_emailTimer=new QTimer(this);
    m_emailTimer->setInterval(EMAIL_DELAY_MS);
    connect(m_emailTimer,SIGNAL(timeout()),this,SLOT(emailErrors()));
}

void StateNotifier::hide(Device *d)
{
    if(!m_hiddenStates.contains(d))
        m_hiddenStates.append(d);
}

void StateNotifier::refresh()
{
    for(int i=0;i<m_list.count();i++)
    {
        m_list[i]->refresh();
    }


    updateCriticity();
}

void StateNotifier::reset()
{
    m_hiddenStates.clear();
    m_list.clear();

}

void StateNotifier::append(HardwareUnit *u)
{
    if(!u || unitState(u))
        return;

    DeviceState* s=new DeviceState(nullptr,u,this);
    append(s);

    append(u->devices());
}

void StateNotifier::append(QList<Device *> dl)
{
    for(int i=0;i<dl.count();i++)
        append(dl[i]);
}

void StateNotifier::append(Device *d)
{
    if(!d || state(d))
        return;

    DeviceState* s=new DeviceState(d,nullptr,this);
    append(s);
}

void StateNotifier::append(DeviceState *s)
{
    if(!s)
        return;

    connect(s,SIGNAL(changed()),this,SLOT(errorSlot()));
    m_list.append(s);
}

DeviceState *StateNotifier::worst()
{

    DeviceState* out=nullptr;
    for(int i=0;i<m_list.count();i++)
    {
        if(!out || out->criticity()<m_list[i]->criticity())
        {
            out=m_list[i];
        }
    }
    return out;
}

void StateNotifier::updateCriticity()
{
    if(m_list.isEmpty())
    {
        m_criticity=DeviceState::Good;
        return;

    }
    DeviceState::Criticity st=worst()->criticity();

    bool diff=st!=m_criticity;
     m_criticity=st;
    if(diff)
    {
        emit stateChanged();
        if(st==DeviceState::Danger && SEND_MAILS)
            m_emailTimer->start();
    }

}

DeviceState *StateNotifier::unitState(HardwareUnit *u)
{

    for(int i=0;i<m_list.count();i++)
        if(m_list[i]->unit()==u)
            return m_list[i];

     return nullptr;
}

DeviceState *StateNotifier::state(Device *s)
{
    for(int i=0;i<m_list.count();i++)
        if(m_list[i]->device()==s)
            return m_list[i];

    return nullptr;
}

QList<DeviceState *> StateNotifier::bads()
{
    QList<DeviceState *> out;
    for(int i=0;i<m_list.count();i++)
        if(m_list[i]->criticity()!=DeviceState::Good)
            out<<m_list[i];

    return out;

}

void StateNotifier::emailErrors()
{
    auto l=bads();
    QStringList out;
    for(int i=0;i<l.count();i++)
    {
        out<<l[i]->fullDiagnosis();
    }

    m_emailTimer->stop();
    EmailNotifier::error(out.join("\n"));
}

void StateNotifier::errorSlot()
{
    updateCriticity();
    emit changed();
}

DeviceState::Criticity StateNotifier::criticity() const
{
    return m_criticity;
}

