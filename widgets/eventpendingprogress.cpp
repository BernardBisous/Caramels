#include "eventpendingprogress.h"

EventPendingProgress::EventPendingProgress(QWidget *parent)
    :ProgressWidget{parent}
{
    QStringList a;
    a<<"Commencer";
    a<<"Repousser";
    a<<"Ignorer";
    addActions(a);

    /*
    ActionWidget* ab=addAction("Commencer");
    connect(ab,SIGNAL(clicked()),this,SLOT(startSlot()));

    ActionWidget* b=addAction("Repousser");
    connect(b,SIGNAL(clicked()),this,SLOT(stop()));

    ActionWidget* b=addAction("Ignorer");
    connect(b,SIGNAL(clicked()),this,SLOT(skipSlot()));
    */
}

void EventPendingProgress::setEvent(Event e)
{
    setText("Evenement en attente: "+e.name,"Il ne tient qu'a vous de le faire maintenant ou plus tard, mais ne trainez par trop !");
    start();
}

void EventPendingProgress::skipSlot()
{
    emit skip();

}

void EventPendingProgress::startSlot()
{
    emit start();
}
