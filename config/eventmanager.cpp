#include "eventmanager.h"
#include "qboxlayout.h"
#include "qdatetime.h"
#include "config/parameter.h"
#include "wizzards/eventwizzard.h"
EventManager::EventManager(QWidget *parent)
    : QWidget{parent},m_tent(nullptr),m_abstracted(true),
      m_eventStart(),m_client(nullptr)
{

    setLayout(new QVBoxLayout);

    m_editor=new ActionWidget;
    m_editor->setLayout(new QVBoxLayout);
    m_editor->layout()->addWidget(m_nameLabel=new QLabel("name"));
    m_editor->setIgnoreClick(true);
    m_editor->setHoverable(false);

    m_editor->setMaximumHeight(80);
    layout()->addWidget(new QLabel("Prochain évenement:"));
    layout()->addWidget(m_editor);


    QWidget* b=new QWidget;
    b->setLayout(new QHBoxLayout);
    b->layout()->setContentsMargins(0,0,0,0);
    b->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    b->layout()->addWidget(m_dateLabel=new QLabel);
    b->layout()->addWidget(m_skipButton=new ToolButton("Skip"));
    b->layout()->addWidget(m_startButton=new ToolButton("Start now"));
    m_editor->layout()->addWidget(b);
    layout()->addWidget(m_listLabel=new QLabel);

    m_emptyLabel=new QWidget;
    m_emptyLabel->setLayout(new QVBoxLayout);
    QLabel* ename=new QLabel("✅ Tout bon !");
    m_emptyLabel->layout()->addWidget(ename);
    QLabel* esub=new QLabel("Plus rien a faire normalement");
    esub->setWordWrap(true);
    m_emptyLabel->layout()->addWidget(esub);

    m_emptyLabel->setHidden(true);
    layout()->addWidget(m_emptyLabel);


    QFont f=font();
    f.setPointSize(14);
    m_nameLabel->setFont(f);
    ename->setFont(f);

    connect(m_skipButton,SIGNAL(clicked()),this,SLOT(skip()));
    connect(m_startButton,SIGNAL(clicked()),this,SLOT(start()));

    m_listLabel->setAlignment(Qt::AlignTop);
}

void EventManager::handle(Events *e, Tent *t)
{
    m_client=e;
    if(t)
    {
        connect(t,SIGNAL(dateChanged(QDateTime)),this,SLOT(setStartedDate(QDateTime)));
        connect(t,SIGNAL(newValue(int)),this,SLOT(refresh()));
    }
    m_tent=t;
    refresh();
}


void EventManager::refresh()
{
    if(!m_tent || !m_client)
        return;
    bool end=( !m_client->count());
    m_emptyLabel->setHidden(!end);
    m_editor->setHidden(end);
    m_listLabel->setHidden(end || m_abstracted);

    if(end)
    {
        m_tent->setEventsDone();
        return;
    }

    QString sl;
    for(int i=0;i<m_client->count();i++) // should find current !
    {
        Event e=m_client->at(i);
        QDateTime t=m_startedDate.addSecs(Parameter::timeMultiplicator()*e.hourIndex);
        if(!i)
        {
            m_dateLabel->setText(t.toString("dd/MM"));
            m_nameLabel->setText(e.name.remove("\n"));

            if(t>QDateTime::currentDateTime())
            {
                 m_editor->setMode(ActionWidget::noBorder);
            }
            else
                 m_editor->setMode(ActionWidget::normal);
        }

        else
        {
            sl+=t.toString("dd/MM")+" : "+e.name;
            if(i<m_client->count()-1)
                sl+="\n";
        }

    }
    m_listLabel->setText(sl);
}

void EventManager::wizzardFinished()
{


    int m=m_eventStart.secsTo(QDateTime::currentDateTime());
    m_eventStart=QDateTime();
    m_client->registerLastEvent(m);
    m_client->skipNext();

    bool end=( !m_client->count());

    if(end)
    {
        m_tent->setEventsDone();
        return;
    }

    refresh();
}

void EventManager::wizzardCanceled()
{

    cancel();
}

void EventManager::showAll(bool s)
{
    m_abstracted=!s;

    if(!s)
    {
        layout()->setContentsMargins(0,0,0,0);
    }
    refresh();

}



QDateTime EventManager::startedDate() const
{
    return m_startedDate;
}

Event EventManager::pending()
{
    Event out{"",0};

    if(!m_startedDate.isValid())
        return out;

    Event e=m_client->next();
    QDateTime t=m_startedDate.addSecs(Parameter::timeMultiplicator()*e.hourIndex);

    if(t<QDateTime::currentDateTime())
    {
        return e;
    }
    return out;
}

void EventManager::setStartedDate(QDateTime newStartedDate)
{
    m_startedDate=newStartedDate;
    if(m_client)
        m_client->reset();
    refresh();
}

void EventManager::skip()
{

    cancel();
    m_tent->console("Event skip "+m_client->list().first().name);
    m_client->skipNext();
    refresh();
}

void EventManager::start()
{
    if(!m_client)
        return;

    EventWizzard* ew=EventWizzard::executeEvent(m_tent,m_client->nextAddr());
    connect(ew,SIGNAL(canceled()),this,SLOT(wizzardCanceled()));
    connect(ew,SIGNAL(finished()),this,SLOT(wizzardFinished()));

    m_eventStart=QDateTime::currentDateTime();
   // m_skipButton->setText("Cancel");
   // m_startButton->setText("Done");
    //m_tent->console("Event skip "+m_client->list().first().name);


    refresh();

}

void EventManager::cancel()
{
    m_eventStart=QDateTime();
    refresh();
}
