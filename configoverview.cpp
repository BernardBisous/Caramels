#include "configoverview.h"
#include "qboxlayout.h"
#include "unitplot.h"

#include <QFileDialog>


ConfigOverview::ConfigOverview(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_topStart(nullptr)
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
    na->layout()->addWidget(m_restartButton=new ToolButton("Restart"));

    m_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_restartButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_progress->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_progress->layout()->setContentsMargins(0,0,0,0);

    center->layout()->addWidget(na);



    QWidget* sp=new QWidget;
    sp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    center->layout()->addWidget(sp);

    center->layout()->addWidget(m_cam=new CameraOverview);

    QFont f=font();
    f.setPointSize(36);
    m_name->setFont(f);



    connect(m_restartButton,SIGNAL(clicked()),this,SLOT(restart()));
}

void ConfigOverview::setTent(Tent *t)
{
    m_client=t;

    if(!t)
        return;

   // m_cam->handle(t->cam());
    connect(t,SIGNAL(newValue(int)),this,SLOT(valueSlot(int)));
    connect(t,SIGNAL(dateChanged(QDateTime)),this,SLOT(updateDate()));
    connect(t,SIGNAL(dateChanged(QDateTime)),m_event,SLOT(setStartedDate(QDateTime)));

    if(t->config())
        m_event->handle(t->config()->events(),t);

    m_state->handle(t->state());
    m_cam->handle(t->cam());
    updateDate();
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

void ConfigOverview::restart()
{
    m_client->restart();

    if(m_topStart)
    {
        delete m_topStart;
        m_topStart=nullptr;
    }
    refresh();

}

void ConfigOverview::updateDate()
{


    if(!m_client->startedDate().isValid())
    {
        if(!m_topStart)
        {
            m_topStart=new ConfigTop(this);
            m_topStart->setText("Tente inactive","Commencer une nouvelle pentation ou exporter les fichiers de la précédente");
            m_topStart->addActions(QStringList()<<"Commencer"<<"Exporter");
            connect(m_topStart,SIGNAL(actionTrigg(QString,ActionWidget*)),this,SLOT(actionTop(QString,ActionWidget*)));
            m_topStart->start();
        }

        return;
    }

    m_event->setStartedDate(m_client->startedDate());

}

void ConfigOverview::valueSlot(int )
{
    refresh();
}

void ConfigOverview::actionTop(QString s, ActionWidget *)
{
    if(s=="Commencer")
    {
        restart();


    }
    else if(s=="Exporter")
    {

        QString initialDirectory = "/home/user"; // Replace with your desired initial directory

           // Open the directory dialog
           QString directory = QFileDialog::getExistingDirectory(
               nullptr,
               "Select a Directory",
               initialDirectory
           );

           m_client->exportAll(directory);
    }
}
