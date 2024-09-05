#include "eventmanager.h"
#include "qboxlayout.h"
#include "qdatetime.h"
#include <parameter.h>
EventManager::EventManager(QWidget *parent)
    : QWidget{parent},m_eventStart(),m_client(nullptr)
{

    setLayout(new QVBoxLayout);

    m_editor=new ActionWidget;
    m_editor->setLayout(new QVBoxLayout);
    m_editor->layout()->addWidget(m_nameLabel=new QLabel("name"));
    m_editor->setIgnoreClick(true);
    m_editor->setHoverable(false);

    m_editor->setMaximumHeight(80);
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

void EventManager::handle(Events *e)
{
    m_client=e;
}

void EventManager::refresh()
{


    bool end=(!m_client || !m_client->count());
    m_emptyLabel->setHidden(!end);
    m_editor->setHidden(end);
    m_listLabel->setHidden(end);

    if(end)
    {
        return;
    }

    QString sl;
    for(int i=0;i<m_client->count();i++)
    {
        Event e=m_client->at(i);
        QDateTime t=m_startedDate.addSecs(Parameter::timeMultiplicator()*e.hourIndex);

        if(!i)
        {
            m_dateLabel->setText(t.toString("dd/MM"));


            if(m_eventStart.isValid())
            {
                m_nameLabel->setText(e.name + "(running)");
            }
            else
                m_nameLabel->setText(e.name);
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



QDateTime EventManager::startedDate() const
{
    return m_startedDate;
}

void EventManager::setStartedDate(QDateTime newStartedDate)
{
    m_startedDate = newStartedDate;

    if(m_client)
        m_client->reset();

    refresh();
}

void EventManager::skip()
{
    if(!m_eventStart.isValid())
    {

         m_startButton->setText("Done");
        m_skipButton->setText("Cancel");
    }
    else
    {
         cancel();
         m_startButton->setText("Start");
         m_skipButton->setText("Skip");
         return;
    }
    m_client->skipNext();
    refresh();
}

void EventManager::start()
{
    if(!m_client)
        return;

    if(m_eventStart.isValid())
    {
        int m=m_eventStart.secsTo(QDateTime::currentDateTime());

        m_eventStart=QDateTime();
        m_startButton->setText("Start");
        m_skipButton->setText("Skip");
        m_client->registerLastEvent(m);
        m_client->skipNext();
    }
    else
    {
        m_eventStart=QDateTime::currentDateTime();
        m_skipButton->setText("Cancel");
        m_startButton->setText("Done");
        qDebug()<<"Start event";
    }

    refresh();

}

void EventManager::cancel()
{
    m_eventStart=QDateTime();
    refresh();
}
