#include "tenteditor.h"
#include "qlineedit.h"

TentEditor::TentEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_currentUnit(-1)
{

    setLayout(new QHBoxLayout);



    QWidget* list=new QWidget;
    list->setLayout(new QVBoxLayout);
    list->layout()->addWidget(new QLabel("Unités de contole:"));
    //list->layout()->addWidget(new QLineEdit);
    list->layout()->addWidget(m_units=new ScrollArea);
    list->layout()->setContentsMargins(0,0,0,0);
    list->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);


    layout()->addWidget(list);
    layout()->addWidget(m_plot=new DevicePlot(this));
    layout()->addWidget(m_editor=new DeviceListWidget(this));



    list->setFixedWidth(200);
    m_editor->setFixedWidth(250);
    connect(m_editor,SIGNAL(edit(int)),this,SLOT(editDevice(int)));
    connect(m_units,SIGNAL(trigger(int,QWidget*)),this,SLOT(listSlot(int,QWidget*)));
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


}

void TentEditor::edit(int index)
{
    if(index<0 || !m_client|| index>=m_client->units().count())
        return;


    HardwareUnit*c=m_client->units()[index];
    m_currentUnit=index;

    m_editor->fillList(c);
    //m_plot->handle(c);

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

int TentEditor::currentUnit()
{
    return m_currentUnit;
}

void TentEditor::editDevice(int i)
{

    m_editor->setChecked(i);
    m_plot->handle(m_client->units()[m_currentUnit]->devices()[i]);
}

void TentEditor::listSlot(int i, QWidget *)
{

    edit(i);

}
