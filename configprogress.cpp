#include "configprogress.h"
#include "qboxlayout.h"

ConfigProgress::ConfigProgress(QWidget *parent)
    : QWidget{parent}
{
    setLayout(new QHBoxLayout);
    layout()->addWidget(m_start=new QLabel);
    layout()->addWidget(m_progress=new ProgressBar);
    layout()->addWidget(m_end=new QLabel);
    m_start->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_end->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_progress->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
}

void ConfigProgress::refresh(GrowConfig *c, QDateTime startDate)
{
    if(!c)
        return;

    int totalSec=c->maxHours()*Parameter::timeMultiplicator();

    m_start->setText(startDate.toString("dd.MM - hh:mm"));
    m_end->setText(startDate.addSecs(totalSec).toString("dd.MM - hh:mm"));

    int elapsed=startDate.secsTo(QDateTime::currentDateTime());
    double v=elapsed;
    v=v/totalSec;
    m_progress->setValue(v);
}

void ConfigProgress::refresh(Tent *t)
{
    if(t && t->config())
    refresh(t->config(),t->startedDate());
}
