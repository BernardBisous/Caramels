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
    na->layout()->addWidget(m_name=new QLabel);
    na->layout()->addWidget(m_restartButton=new ToolButton("Restart"));
    m_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_restartButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    center->layout()->addWidget(na);
    center->layout()->addWidget(m_progress=new ConfigProgress);
    m_progress->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_progress->layout()->setContentsMargins(0,0,0,0);

    QWidget*rw=new QWidget;
    rw->setLayout(new QHBoxLayout);


    QWidget* plots=new QWidget;
    plots->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_plotLayout=new QGridLayout;
    plots->setLayout(m_plotLayout);
    m_plotLayout->addWidget(m_event=new EventManager,0,0);


     rw->layout()->addWidget(plots);
    // rw->layout()->addWidget(m_results=new ResultsWidget);
    // rw->layout()->addWidget(m_injection=new InjectionWidget);

    center->layout()->addWidget(rw);

    m_plotLayout->setContentsMargins(0,0,0,0);
    m_plotLayout->setSpacing(0);

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

    connect(t,SIGNAL(newValue(int)),this,SLOT(valueSlot(int)));
    connect(t,SIGNAL(dateChanged(QDateTime)),this,SLOT(updateDate()));
    connect(t,SIGNAL(dateChanged(QDateTime)),m_event,SLOT(setStartedDate(QDateTime)));

    if(t->config())
        m_event->handle(t->config()->events());



    /*

    m_plotLayout->addWidget(plot(t->temperatures()),0,1);
    m_plotLayout->addWidget(plot(t->co2Manager()),0,2);
    m_plotLayout->addWidget(plot(t->lights()),1,0);
    m_plotLayout->addWidget(plot(t->pumps()),1,1);
    m_plotLayout->addWidget(plot(t->phManager()),1,2);
    */

   // m_injection->handle(t);
   // m_results->handle(t);


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
  //  m_injection->updatePlot();

}

UnitPlot *ConfigOverview::plot(HardwareUnit *u)
{
    UnitPlot*p=new UnitPlot;
    m_plots.append(p);
    p->removeMargins();
    p->handle(u);
    connect(p,SIGNAL(clicked()),this,SLOT(plotTrigg()));
    return p;
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
    for(int i=0;i<m_plots.count();i++)
        m_plots[i]->setStartDate(m_client->startedDate());
}

void ConfigOverview::valueSlot(int i)
{
  //  qDebug()<<"refreshConfig"<<i;
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

void ConfigOverview::plotTrigg()
{

    UnitPlot* t=dynamic_cast<UnitPlot*>(sender());
    if(t)
        emit edit(t->client());
}
