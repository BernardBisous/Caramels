#include "devicelistwidget.h"
#include "deviceeditor.h"

DeviceListWidget::DeviceListWidget(QWidget *parent)
    : ScrollArea{false,parent}
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
        addWidget(a);
    }
    addSpacer();
}

void DeviceListWidget::fillList(HardwareUnit *u)
{
    fillList(u->devices());
}

void DeviceListWidget::setChecked(int index)
{

    auto l=widgets();
    for(int i=0;i<l.count();i++)
    {
        DeviceEditor* wa=dynamic_cast<DeviceEditor*>(l[i]);
        if(wa)
        {
            wa->setAbstracted(i!=index);
        }
    }
}

void DeviceListWidget::listSlot(int i, QWidget *)
{
    emit edit(i);
}
