#include "overview.h"
#include "qboxlayout.h"

Overview::Overview(QWidget *parent)
    : QWidget{parent},m_tent(nullptr)
{
    setLayout(new QHBoxLayout);
    layout()->addWidget(m_hardware=new HardwareOverview);
    layout()->addWidget(m_config=new ConfigOverview);
    layout()->addWidget(m_console=new ConsoleWidget);

    m_config->layout()->setContentsMargins(0,0,0,0);

    m_hardware->setBackgroundRole(QPalette::Window);
    m_hardware->setFixedWidth(200);
    m_console->setFixedWidth(300);

    connect(m_config,SIGNAL(edit(HardwareUnit*)),this,SLOT(editSlot(HardwareUnit*)));
}

void Overview::loadHardware(Tent *t)
{
    m_tent=t;

    if(!t)
        return;

    m_hardware->handle(t);
    m_config->setTent(t);
    m_console->setTent(t);

}



void Overview::createSlot()
{

}
