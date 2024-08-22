#include "configeditor.h"
#include "qboxlayout.h"

ConfigEditor::ConfigEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{
    setLayout(new QHBoxLayout);

    layout()->addWidget(m_list=new ParameterListWidget);
    layout()->addWidget(m_editor=new ParameterEditor);

    m_list->setMaximumWidth(250);
m_list->setMinimumWidth(150);
    connect(m_list,SIGNAL(addOne()),this,SLOT(addSlot()));
    connect(m_list,SIGNAL(selected(int)),this,SLOT(editParameter(int)));
}

void ConfigEditor::handle(GrowConfig *t)
{

    m_client=t;
    m_list->handle(t);

    editParameter(0);
}

void ConfigEditor::loadData()
{

}

void ConfigEditor::editParameter(int i)
{

    if(!m_client || i>=m_client->countParameters() || i<0)
        return;

    auto a=m_client->parameterAddr(i);
    if(!a)
        return;

    m_editor->handle(a);
    m_list->setSelected(i);

    m_editor->setXRange(m_client->maxHours());
}

void ConfigEditor::addSlot()
{
    if(!m_client->browse())
        return;

    m_list->refreshList();
    editParameter(m_client->countParameters()-1);

}
