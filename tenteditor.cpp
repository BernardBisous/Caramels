#include "tenteditor.h"
#include "qlineedit.h"

TentEditor::TentEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{

    setLayout(new QHBoxLayout);



    QWidget* list=new QWidget;
    list->setLayout(new QVBoxLayout);
    list->layout()->addWidget(new QLabel("Périphériques disponibles:"));
    //list->layout()->addWidget(new QLineEdit);
    list->layout()->addWidget(m_devices=new ScrollArea);
    list->layout()->setContentsMargins(0,0,0,0);
    list->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);


    layout()->addWidget(list);
    layout()->addWidget(m_plot=new DevicePlot(this));

    QWidget* edit=new QWidget;
    edit->setLayout(new QVBoxLayout);
    edit->layout()->addWidget(m_editor=new DeviceEditor(this));
    QWidget*space=new QWidget;
    space->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    edit->layout()->addWidget(space);
    edit->layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(edit);



    list->setFixedWidth(200);
    edit->setFixedWidth(250);
    connect(m_devices,SIGNAL(trigger(int,QWidget*)),this,SLOT(listSlot(int,QWidget*)));
}

void TentEditor::handle(Tent *t)
{
    m_client=t;

    auto l=m_client->devices();

    m_devices->clear();
    for(int i=0;i<l.count();i++)
    {
        m_devices->addActionText(l[i]->name());
    }
    m_devices->addSpacer();

    if(!l.isEmpty())
        edit(0);


}

void TentEditor::edit(int index)
{
    if(index<0 || !m_client|| index>=m_client->devices().count())
        return;


    Device*c=m_client->devices()[index];
    m_editor->handle(c);
    m_plot->handle(c);

    auto l=m_devices->widgets();
    for(int i=0;i<l.count();i++)
    {
        ActionWidget* wa=dynamic_cast<ActionWidget*>(l[i]);
        if(wa)
        {
            wa->setChecked(i==index);
        }
    }


}

void TentEditor::edit(Device *s)
{
    edit(m_client->indexOf(s));
}

void TentEditor::listSlot(int i, QWidget *)
{

    edit(i);

}
