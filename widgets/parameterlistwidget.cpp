#include "parameterlistwidget.h"
#include "Interface/toolbutton.h"
#include "Interface/actionwidget.h"
#include "qboxlayout.h"
#include "qlabel.h"

ParameterListWidget::ParameterListWidget(QWidget *parent)
    : QWidget{parent},m_current(-1)
{
    setLayout(new QVBoxLayout);


    layout()->addWidget(new QLabel("Monitored parameters:"));
    layout()->addWidget(m_list=new ScrollArea);

    connect(m_list, SIGNAL(trigger(int,QWidget*)),this,SLOT(trigSlot(int,QWidget*)));
}

void ParameterListWidget::handle(GrowConfig *c)
{

    int n=currentSelected();

    if(!c)
        return;

     m_list->fillList(c->parameterNames());
     if(n>=0)
         setSelected(n);
}

void ParameterListWidget::handleHadware(HardwareUnit *h)
{
    if(!h)
        return;

    setParams(h->parameters());
}

void ParameterListWidget::setSelected(int index)
{

    m_current=index;
    QList<ActionWidget *> l=items();
    for(int i=0;i<l.count();i++)
    {
        l[i]->setChecked(i==m_current && m_current>=0);
    }

    if(index<0 || index>=m_params.count())
        emit selected(index,nullptr);
    else
        emit selected(index,m_params[index]);
}



int ParameterListWidget::currentSelected()
{
    QList<ActionWidget *> l=items();
    for(int i=0;i<l.count();i++)
    {
        if(l[i]->checked())
            return i;
    }
    return -1;
}

QList<ActionWidget *> ParameterListWidget::items()
{

    auto l=m_list->widgets();
  QList<ActionWidget *>out;
    for(int i=0;i<l.count();i++)
    {
        auto a=dynamic_cast<ActionWidget*>(l[i]);
        if(a)
            out<<a;
    }
    return out;
}

void ParameterListWidget::trigSlot(int i, QWidget*)
{

    qDebug()<<"selected()"<<i<<m_current;

    if(i<0 || i>m_params.count() || i==m_current)
    {
       setSelected(-1);
    }

    else
        setSelected(i);



}

int ParameterListWidget::current() const
{
    return m_current;
}

void ParameterListWidget::setCurrent(int newCurrent)
{
    m_current = newCurrent;
}

QList<Parameter *> ParameterListWidget::params() const
{
    return m_params;
}

void ParameterListWidget::setParams(const QList<Parameter *> &newParams)
{
    int n=currentSelected();

    m_params = newParams;

    m_list->fillList(paramNames());
    if(n>=0)
        setSelected(n);
}

QStringList ParameterListWidget::paramNames()
{
    QStringList out;

    for(int i=0;i<m_params.count();i++)
        out<<m_params[i]->name();

    return out;
}


