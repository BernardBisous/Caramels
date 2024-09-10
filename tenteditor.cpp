#include "tenteditor.h"

TentEditor::TentEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_currentUnit(-1)
{

    setLayout(new QHBoxLayout);
    QWidget* list=new QWidget;
    list->setLayout(new QVBoxLayout);
    list->layout()->addWidget(m_ports=new SerialEditor);
    list->layout()->addWidget(new QLabel("Unités de contole:"));
    list->layout()->addWidget(m_units=new ScrollArea);
    list->layout()->addWidget(m_parameters=new ParameterListWidget);
    list->layout()->setContentsMargins(0,0,0,0);
    list->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    m_units->setContentsMargins(0,0,0,0);
    m_parameters->layout()->setContentsMargins(0,0,0,0);
    list->setMaximumWidth(200);

    layout()->addWidget(list);
    layout()->addWidget(m_editor=new UnitEditor);


    connect(m_units,SIGNAL(trigger(int,QWidget*)),this,SLOT(listSlot(int,QWidget*)));
    connect(m_parameters,SIGNAL(selected(int,Parameter*)),this,SLOT(paramListSlot(int,Parameter*)));
}

void TentEditor::handle(Tent *t)
{
    m_client=t;

    auto l=m_client->units();

    m_units->clear();
    for(int i=0;i<l.count();i++)
    {
        m_units->addActionText(l[i]->name());
    }
    m_units->addSpacer();

    if(!l.isEmpty())
        edit(0);

    m_ports->handle(t);

}

void TentEditor::edit(int index)
{
    if(index<0 || !m_client|| index>=m_client->units().count())
        return;


    HardwareUnit*c=m_client->units()[index];
    m_currentUnit=index;
    m_editor->handle(c);
    m_parameters->setHidden(c->parameters().isEmpty());
    m_parameters->handleHadware(c);

    auto l=m_units->widgets();
    for(int i=0;i<l.count();i++)
    {
        ActionWidget* wa=dynamic_cast<ActionWidget*>(l[i]);
        if(wa)
        {
            wa->setChecked(i==index);
        }
    }
}

void TentEditor::edit(HardwareUnit *s)
{
    edit(m_client->indexOf(s));
}

HardwareUnit* TentEditor::currentUnit()
{
    if(!m_client || m_client->units().count()<=m_currentUnit || m_currentUnit<0)
        return nullptr;

    return m_client->units()[m_currentUnit];
}



void TentEditor::listSlot(int i, QWidget *)
{

    edit(i);

}

void TentEditor::paramListSlot(int , Parameter *p)
{


    m_editor->editParameter(p);

}

