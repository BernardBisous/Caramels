#include "uniteditor.h"


UnitEditor::UnitEditor(QWidget *parent)
    : QWidget{parent},m_overview(nullptr)
{
    setLayout(new QHBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(20);
    QWidget* edit=new QWidget;
    edit->setLayout(new QVBoxLayout);
    edit->layout()->setContentsMargins(0,0,0,0);
    edit->layout()->addWidget(m_devices=new DeviceListWidget);


    edit->layout()->addWidget(m_paramEditor=new ParameterValueEditor);


    layout()->addWidget(m_overview=new UnitOverview);
    connect(m_paramEditor,SIGNAL(changed()),m_overview->parameter(),SLOT(refresh()));
    connect(m_overview,SIGNAL(requestDeviceEdit(Device*)),this,SLOT(editDevice(Device*)));
    connect(m_overview,SIGNAL(requestParamEdit(Parameter*)),this,SLOT(editParameter(Parameter*)));
    m_paramEditor->setPlot(m_overview->parameter());

    m_paramEditor->hide();
    layout()->addWidget(edit);


    m_devices->removeMargins();
    m_devices->setFixedWidth(250);

    connect(m_devices,SIGNAL(edit(int)),this,SLOT(editDevice(int)));

    m_devices->setHidden(true);
}

void UnitEditor::handle(HardwareUnit *h)
{
    m_client=h;
    m_devices->fillList(h );

    m_overview->handle(h);
    m_paramEditor->setHidden(true);
}

void UnitEditor::showDevices(bool s)
{
    m_devices->setHidden(!s);
}

void UnitEditor::showDevices()
{
    showDevices(m_devices->isHidden());
}






void UnitEditor::editDevice(int i)
{
    auto c=m_client;
    if(i<0 || !c || i>=c->devices().count())
        return;

    m_devices->setChecked(i);
    m_overview->editDevice(c->devices()[i]);
    m_paramEditor->setHidden(true);
    m_devices->setHidden(false);
}

void UnitEditor::editParameter(Parameter *p)
{
    m_paramEditor->setClient(p);
    m_paramEditor->setHidden(!p);



    m_devices->setHidden(true);
    m_devices->setChecked(-1);
    m_overview->editParameter(p);

    auto ps=m_overview->paramPlot(p);
    if(ps)
        m_paramEditor->setPlot(ps);
}

void UnitEditor::editDevice(Device *d)
{
    if(!m_client)
        return;

    int i=m_client->devices().indexOf(d);
    if(i>=0)
        editDevice(i);
}

ParameterValueEditor *UnitEditor::paramEditor() const
{
    return m_paramEditor;
}

