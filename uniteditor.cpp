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
    edit->layout()->addWidget(m_parametersWidget=new QWidget);

    m_parametersWidget->setLayout(new QVBoxLayout);
    m_parametersWidget->layout()->addWidget(new QLabel("Parametres associés:"));
    m_parametersWidget->layout()->addWidget(m_parameters=new ScrollArea);
    m_parametersWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    layout()->addWidget(m_devicePlot=new DevicePlot);
    layout()->addWidget(edit);

    edit->setFixedWidth(250);
    connect(m_devices,SIGNAL(edit(int)),this,SLOT(editDevice(int)));
    connect(m_parameters,SIGNAL(trigger(int,QWidget*)),this,SLOT(paramSlot(int,QWidget*)));
}

void UnitEditor::handle(HardwareUnit *h)
{
    m_client=h;
    m_parameters->fillList(h->parametersName());
    m_parametersWidget->setHidden(h->parameters().isEmpty());
    m_devices->fillList(h );
    editDevice(0);


}

void UnitEditor::editDevice(int i)
{
    auto c=m_client;
    if(!c || i>=c->devices().count())
        return;

    qDebug()<<"edit dev"<<i;

    m_devices->setChecked(i);
    m_devicePlot->handle(c->devices()[i]);
}



void UnitEditor::paramSlot(int i, QWidget *)
{


    if(i>=0 && i<m_client->parameters().count())
        emit editParameter(m_client->parameters()[i]);
}
