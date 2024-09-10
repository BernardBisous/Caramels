#include "uniteditor.h"
#include "qlabel.h"

UnitEditor::UnitEditor(QWidget *parent)
    : QWidget{parent}
{
    setLayout(new QHBoxLayout);
    QWidget* edit=new QWidget;
    edit->setLayout(new QVBoxLayout);
    edit->layout()->setContentsMargins(0,0,0,0);
    edit->layout()->addWidget(m_devices=new DeviceListWidget);
    edit->layout()->addWidget(m_paramEditor=new ParameterValueEditor);
    layout()->addWidget(m_plot=new UnitPlot);
    m_plot->setContentsMargins(0,0,0,0);
    layout()->addWidget(edit);
    edit->setFixedWidth(250);
    connect(m_devices,SIGNAL(edit(int)),this,SLOT(editDevice(int)));
    connect(m_paramEditor,SIGNAL(changed()),m_plot,SLOT(updateParameters()));
}

void UnitEditor::handle(HardwareUnit *h)
{
    m_client=h;
    m_devices->fillList(h );
    m_plot->handle(h);
    editParameter(nullptr);

}

void UnitEditor::editDevice(int i)
{
    auto c=m_client;
    if(!c || i>=c->devices().count())
        return;


    m_devices->setChecked(i);


    //m_devicePlot->handle(c->devices()[i]);
}

void UnitEditor::editParameter(Parameter *p)
{
    m_paramEditor->setClient(p);
    m_paramEditor->setHidden(!p);
    m_devices->setChecked(-1);


    m_paramEditor->setSeries(m_plot->paramSerie(p));



}

