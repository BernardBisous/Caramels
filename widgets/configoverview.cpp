#include "configoverview.h"

#include <QFileDialog>
#include "qboxlayout.h"
#include "wizzards/startwizzard.h"
#include "widgets/eventpendingprogress.h"

ConfigOverview::ConfigOverview(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_topStart(nullptr),m_eventStart(nullptr)
{
    setLayout(new QHBoxLayout);
    QWidget* center=new QWidget;

    center->setLayout(new QVBoxLayout);
    layout()->addWidget(center);


    QWidget* na=new QWidget;
    na->setLayout(new QHBoxLayout);
    na->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    na->layout()->setSpacing(20);


    na->layout()->addWidget(m_state=new StateWidget);

    QWidget*nw=new QWidget;
    nw->setLayout(new QVBoxLayout);
    nw->layout()->setContentsMargins(0,0,0,0);
    nw->layout()->addWidget(m_name=new QLabel);
    nw->layout()->addWidget(m_progress=new ConfigProgress);

    na->layout()->addWidget(nw);
    na->layout()->addWidget(m_event=new EventManager);

    m_event->showAll(false);
    m_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_progress->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_progress->layout()->setContentsMargins(0,0,0,0);

    center->layout()->addWidget(na);


    m_central=new QWidget;
    m_central->setLayout(new QVBoxLayout);
    center->layout()->addWidget(m_central);
    m_central->layout()->addWidget(m_cam=new WebcamWidget);
    m_central->layout()->setContentsMargins(0,0,0,0);

    QFont f=font();
    f.setPointSize(36);
    m_name->setFont(f);
}

void ConfigOverview::setTent(Tent *t)
{
    m_client=t;

    if(!t)
        return;

    connect(t,SIGNAL(newValue(int)),this,SLOT(valueSlot(int)));
    connect(t,SIGNAL(dateChanged(QDateTime)),this,SLOT(updateDate()));
    connect(t,SIGNAL(dateChanged(QDateTime)),m_event,SLOT(setStartedDate(QDateTime)));

    if(t->config())
        m_event->handle(t->config()->events(),t);

    m_state->handle(t->state());
    m_cam->handle(t->cam());
    updateDate();
    updateEvent();
    refresh();
}

void ConfigOverview::refresh()
{
    if(!m_client)
        return;

    auto b=m_client->config();
    if(!b)
        return;

    m_name->setText(b->name());
    m_progress->refresh(b,m_client->startedDate());
    m_state->refresh();

}

void ConfigOverview::presentEvent(Event e)
{

    if(!m_eventStart)
    {
        m_eventStart=new EventPendingProgress(m_central);
        connect(m_eventStart,SIGNAL(actionTrigg(QString,ActionWidget*)),this,SLOT(eventAction(QString,ActionWidget*)));
    }

    m_eventStart->setEvent(e);
}

void ConfigOverview::updateEvent()
{


    Event e=m_event->pending();
    if(e.name.isEmpty())
    {
        if(m_eventStart)
            m_eventStart->stop();
        return;
    }

    presentEvent(e);

}

void ConfigOverview::restart()
{
    StartWizzard::execute(m_client);
}

void ConfigOverview::updateDate()
{


    if(!m_client->startedDate().isValid())
    {
        if(!m_topStart)
        {
            m_topStart=new ProgressWidget(this);
            m_topStart->setText("Tente inactive","Etes-vous pret à lancer une nouvelle plantation ?");
            m_topStart->addActions(QStringList()<<"Commencer");
            connect(m_topStart,SIGNAL(actionTrigg(QString,ActionWidget*)),this,SLOT(actionTop(QString,ActionWidget*)));
            m_topStart->start();
        }

        return;
    }

    if(m_topStart)
    {
        m_topStart->stop();
    }

    m_event->setStartedDate(m_client->startedDate());
    updateEvent();

}

void ConfigOverview::valueSlot(int)
{
    refresh();
    updateEvent();
}

void ConfigOverview::actionTop(QString s, ActionWidget *)
{
    if(s=="Commencer")
    {
        restart();
    }
}

void ConfigOverview::eventAction(QString s, ActionWidget *)
{
    //TODO , clean this using ids not strings

    if(s=="Commencer")
    {
        m_event->start();
    }
    else if (s=="Ignorer")
    {
        m_event->skip();
    }

    m_eventStart->stop();
    m_eventStart=nullptr;
}
