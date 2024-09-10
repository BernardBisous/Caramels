#include "devicelistwidget.h"
#include "deviceeditor.h"

DeviceListWidget::DeviceListWidget(QWidget *parent)
    : ScrollArea{false,parent},m_current(-1)
{
    connect(this,SIGNAL(trigger(int,QWidget*)),this,SLOT(listSlot(int,QWidget*)));
}

void DeviceListWidget::fillList(QList<Device *> l)
{
    clear();
    for(int i=0;i<l.count();i++)
    {
        auto a=new DeviceEditor;
        a->handle(l[i]);
        a->setAbstracted(true);
        addActionWidget(a);
    }
    addSpacer();
}

void DeviceListWidget::fillList(HardwareUnit *u)
{
    fillList(u->devices());
}

void DeviceListWidget::setChecked(int index)
{

    m_current=index;
    auto l=widgets();
    for(int i=0;i<l.count();i++)
    {
        DeviceEditor* wa=dynamic_cast<DeviceEditor*>(l[i]);
        if(wa)
        {

            if(i==index && index>0)
                wa->setMode(ActionWidget::normal);

            else
                wa->setMode(ActionWidget::noBorder);

            wa->setAbstracted(i!=index);
        }
    }

    emit checked(index);
}



void DeviceListWidget::listSlot(int i, QWidget *)
{
    if(m_current==i)
        edit(-1);

    else emit edit(i);
}

int DeviceListWidget::current() const
{
    return m_current;
}

void DeviceListWidget::setCurrent(int newCurrent)
{
    m_current = newCurrent;
}
