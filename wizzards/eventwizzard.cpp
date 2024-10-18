#include "eventwizzard.h"
#include "hardware/tent.h"
#include "qapplication.h"
#define PATH_FILES "events"
#include "constants.h"

EventWizzard::EventWizzard(Tent *t, Event *e, QWidget *parent)
    : Wizzard{parent},m_client(e),m_tente(t)
{
    m_deleteAtEnd=true;
    loadEventName(e->name);


}

EventWizzard* EventWizzard::executeEvent(Tent *t, Event *e)
{
    if(e)
    {
        EventWizzard*w=new EventWizzard(t,e,QApplication::activeWindow());

        w->start();

        return w;
    }
    return nullptr;
}

void EventWizzard::loadEventName(QString name)
{
    setName(name+":");
    load(name+".json",QString(PATH_FILES)+"/");
}



ArchiveSequence::ArchiveSequence(Tent *e, QWidget *parent):
    WizzardSequence(false,parent),m_tente(e)
{
    setName("Archivage");
    setTexts("Concluons la récolte avec quelques données",
             "Cela est la dernière étape ! Tout devrait être terminé:\n - Le produit est conditionné\n - La tente est propre et prette à recommancer");


}
