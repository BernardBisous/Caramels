#include "uniteditor.h"
#include "qlabel.h"

#include "hardware/waterlevelmanager.h"
#include "unitoverviews.h"

UnitEditor::UnitEditor(QWidget *parent)
    : QWidget{parent},m_overview(nullptr)
{
    setLayout(new QHBoxLayout);
    QWidget* edit=new QWidget;
    edit->setLayout(new QVBoxLayout);
    edit->layout()->setContentsMargins(0,0,0,0);
    edit->layout()->addWidget(m_devices=new DeviceListWidget);
    edit->layout()->addWidget(m_paramEditor=new ParameterValueEditor);


    layout()->addWidget(m_overview=new UnitOverview);
     connect(m_paramEditor,SIGNAL(changed()),m_overview->parameter(),SLOT(refresh()));

     connect(m_overview,SIGNAL(requestDeviceEdit(Device*)),this,SLOT(editDevice(Device*)));
     connect(m_overview,SIGNAL(requestParamEdit(Parameter*)),this,SLOT(editParameter(Parameter*)));

    m_paramEditor->setSeries(m_overview->parameter()->series());
    m_paramEditor->hide();
    layout()->addWidget(edit);
    edit->setFixedWidth(250);
    connect(m_devices,SIGNAL(edit(int)),this,SLOT(editDevice(int)));
}

void UnitEditor::handle(HardwareUnit *h)
{
    m_client=h;
    m_devices->fillList(h );
    m_overview->handle(h);
}






void UnitEditor::editDevice(int i)
{
    auto c=m_client;
    if(!c || i>=c->devices().count())
        return;

    m_devices->setChecked(i);
    m_overview->editDevice(c->devices()[i]);
    m_paramEditor->setHidden(true);
}

void UnitEditor::editParameter(Parameter *p)
{
    m_paramEditor->setClient(p);
    m_paramEditor->setHidden(!p);

    m_devices->setChecked(-1);
    m_overview->editParameter(p);
}

void UnitEditor::editDevice(Device *d)
{
    if(!m_client)
        return;

    int i=m_client->devices().indexOf(d);
    if(i>=0)
        editDevice(i);
}

