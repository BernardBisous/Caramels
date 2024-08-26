#include "parameterlistwidget.h"
#include "Interface/toolbutton.h"
#include "Interface/actionwidget.h"
#include "qboxlayout.h"
#include "qlabel.h"

ParameterListWidget::ParameterListWidget(QWidget *parent)
    : QWidget{parent}
{
    setLayout(new QVBoxLayout);


    layout()->addWidget(new QLabel("Monitored parameters:"));
    layout()->addWidget(m_list=new ScrollArea);
    layout()->addWidget(m_add=new ToolButton("Load Csv"));


    connect(m_add,SIGNAL(clicked()),this,SLOT(addSlot()));
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
    int n=currentSelected();

    if(!h)
        return;

     m_list->fillList(h->parametersName());
     if(n>=0)
         setSelected(n);
}

void ParameterListWidget::setSelected(int index)
{
    QList<ActionWidget *> l=items();
    for(int i=0;i<l.count();i++)
    {
        l[i]->setChecked(i==index);
    }

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

void ParameterListWidget::trigSlot(int i, QWidget *w)
{
    setSelected(i);
    (void)w;

    emit selected(i);
}


