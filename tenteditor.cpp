#include "tenteditor.h"
#include "qlineedit.h"
#include "Interface/toolbutton.h"

TentEditor::TentEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_currentUnit(-1)
{

    setLayout(new QHBoxLayout);



    QWidget* list=new QWidget;
    list->setLayout(new QVBoxLayout);
    list->layout()->addWidget(m_ports=new MenuButton);
    list->layout()->addWidget(new QLabel("Unités de contole:"));
    //list->layout()->addWidget(new QLineEdit);
    list->layout()->addWidget(m_units=new ScrollArea);

    list->layout()->setContentsMargins(0,0,0,0);
    list->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);


    layout()->addWidget(list);
    layout()->addWidget(m_editor=new UnitEditor);


    m_ports->setPrintActive(false);
    list->setFixedWidth(200);

    connect(m_ports,SIGNAL(actionTrigg(QString)),this,SLOT(portSlot(QString)));
    connect(m_units,SIGNAL(trigger(int,QWidget*)),this,SLOT(listSlot(int,QWidget*)));
    connect(m_editor,SIGNAL(editParameter(Parameter*)),this,SLOT(paramListSlot(Parameter*)));
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



    m_ports->clear();
    m_ports->addActions(t->availablePorts());

    if(t->connected())
    {
        m_ports->setText("✅ "+t->serialPort());
    }
    else
    {
        m_ports->setText("Select port");

    }
}

void TentEditor::edit(int index)
{
    if(index<0 || !m_client|| index>=m_client->units().count())
        return;


    HardwareUnit*c=m_client->units()[index];
    m_currentUnit=index;
    m_editor->handle(c);

    auto l=m_units->widgets();
    for(int i=0;i<l.count();i++)
    {
        ActionWidget* wa=dynamic_cast<ActionWidget*>(l[i]);
        if(wa)
        {
            wa->setChecked(i==index);
        }
    }

    /*

    while(m_parameters->layout()->count())
    {
        auto i=m_parameters->layout()->takeAt(0);
        if(i->widget())
            delete i->widget();
        delete i;
    }
    auto lp=c->parameters();
    for(int i=0;i<lp.count();i++)
    {
        ToolButton*a=new ToolButton(lp[i]->name());
        connect(a,SIGNAL(clicked()),this,SLOT(paramListSlot()));
        m_parameters->layout()->addWidget(a);
    }

    editDevice(0);
    */



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

void TentEditor::paramListSlot(Parameter *p)
{

    qDebug()<<"editww"<<p->name();
    emit editParam(p);

}

void TentEditor::portSlot(QString s)
{
    m_client->setSerialPort(s);
}
