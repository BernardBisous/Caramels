#include "eventwizzard.h"
#include "qapplication.h"
#define PATH_FILES "events"
EventWizzard::EventWizzard(Event *e, QWidget *parent)
    : Wizzard{parent},m_client(e)
{
    m_deleteAtEnd=true;
    loadEventName(e->name);

}

EventWizzard* EventWizzard::executeEvent(Event *e)
{
    if(e)
    {
        EventWizzard*w=nullptr;
        if(e->type==1)
            w=new FinalWizzard(e,QApplication::activeWindow());
        else
            w=new EventWizzard(e,QApplication::activeWindow());

        w->start();

        return w;
    }
    return nullptr;
}

void EventWizzard::loadEventName(QString name)
{
    load(name+".json",QString(PATH_FILES)+"/");
}

FinalWizzard::FinalWizzard(Event *e, QWidget *parent):EventWizzard(e,parent)
{
    qDebug()<<"Loaded finL ";
}
