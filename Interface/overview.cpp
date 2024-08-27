#include "overview.h"
#include "qboxlayout.h"

Overview::Overview(QWidget *parent)
    : QWidget{parent},m_tent(nullptr)
{
    setLayout(new QHBoxLayout);
    layout()->addWidget(m_hardware=new HardwareOverview);
    layout()->addWidget(m_config=new ConfigOverview);
}

void Overview::loadHardware(Tent *t)
{
    m_tent=t;

    if(!t)
        return;

    m_hardware->handle(t);
    m_config->handle(t->config());
}

TopWidget *Overview::emptyWidget()
{
    return nullptr;
}

void Overview::createSlot()
{

}
