#include "parametereditor.h"

ParameterEditor::ParameterEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{

    setLayout(new QHBoxLayout);

    layout()->addWidget(m_plot=new ParameterPlot);

    m_editWidget=new QWidget;
    m_editWidget->setLayout(new QVBoxLayout);
    m_editWidget->layout()->addWidget(m_name=new NameLabel);
    layout()->addWidget(m_editWidget);

    m_editWidget->setFixedWidth(200);
    connect(m_plot,SIGNAL(clicked(QPointF)),this,SLOT(pointClickedSlot(QPointF)));
    connect(m_name,SIGNAL(edited(QString)),this,SLOT(nameChangedSlot(QString)));

}

void ParameterEditor::handle(Parameter *p)
{


    m_client=p;

    if(!p)
        return;

    m_plot->refresh(p);
    m_name->load(p->name());
}

void ParameterEditor::setXRange(int t)
{
    m_plot->setXrange(t);
}

void ParameterEditor::refresh()
{
    m_plot->refresh(m_client);

}

void ParameterEditor::nameChangedSlot(QString s)
{
    m_client->setName(s);
}

void ParameterEditor::pointClickedSlot(QPointF p)
{

    TimedValue v;
    v.hourIndex=p.x();
    v.value=p.y();

    int i=m_client->append(v);

    m_plot->refresh(m_client);
    m_plot->select(i);

}
