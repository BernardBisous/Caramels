#include "unitoverview.h"
#include "qboxlayout.h"

UnitOverview::UnitOverview(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_integral(nullptr)
{
    setLayout(new QVBoxLayout);
    QWidget* top=new QWidget;
    top->setLayout(new QHBoxLayout);
    top->layout()->setContentsMargins(0,0,0,0);
    top->layout()->addWidget(m_name=new QLabel);
    top->layout()->addWidget(m_buttons=new QWidget);
    top->layout()->addWidget(m_regulator=new RegulatorWidget);


    layout()->addWidget(top);
    layout()->addWidget(m_desc=new QLabel);
    top->layout()->setContentsMargins(0,0,0,0);
    m_desc->setWordWrap(true);
    m_desc->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_buttons->setLayout(new QHBoxLayout);
    m_buttons->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    m_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    top->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout()->addWidget(m_stack=new QStackedWidget);

    m_stack->addWidget(m_overviewWidget=new QWidget);
    m_stack->addWidget(m_parameter=new ParameterPlot);
    m_stack->addWidget(m_device=new DevicePlot);


    m_overviewWidget->setLayout(new QVBoxLayout);

    QWidget* wp=new QWidget;
    wp->setLayout(new QHBoxLayout);
    wp->layout()->setContentsMargins(0,0,0,0);
    wp->layout()->addWidget(m_integral=new IntegralPlot);
    wp->layout()->addWidget(m_parametersWidget=new QWidget);

    m_parametersWidget->setLayout(new QVBoxLayout);

    m_overviewWidget->layout()->addWidget(wp);
    m_overviewWidget->layout()->addWidget(m_devicesWidget=new QWidget);
    m_devicesWidget->setLayout(new QHBoxLayout);


    m_devicesWidget->layout()->setContentsMargins(0,0,0,0);
    m_parametersWidget->layout()->setContentsMargins(0,0,0,0);
    m_stack->setCurrentWidget(m_overviewWidget);


    QFont f=font();
    f.setPointSize(18);
    m_name->setFont(f);

}

void UnitOverview::handle(HardwareUnit *u)
{
    m_client=u;


    while(!m_devices.isEmpty())
        delete m_devices.takeFirst();

    while(!m_parameters.isEmpty())
        delete m_parameters.takeFirst();

    while(!m_buttonList.isEmpty())
        delete m_buttonList.takeFirst();

    if(!u)
        return;

    m_name->setText(u->name());

    printIntegral(u->interestingIntegrals());
    printParameters(u->parameters(),u->startTime());
    printDevices(u->interestingDevices());
    printButtons(u->trigKeys());

    m_regulator->handle(u->regulatingSensor());
    showCentral();
    refresh();

}

void UnitOverview::refresh()
{
    if (!m_client)
        return;

    m_name->setText(m_client->name());
    m_desc->setHidden(m_client->description().isEmpty());
    m_desc->setText(m_client->description());

    if(m_stack->currentWidget()==m_device)
        m_device->updatePlot();

    else if(m_stack->currentWidget()==m_parameter)
        m_parameter->refresh();

    else refreshCentral();
}

void UnitOverview::printIntegral(QList<Actuator *> d)
{

    m_integral->handle(d);
    m_integral->setHidden(d.isEmpty());
}

void UnitOverview::printParameter(Parameter *p,QDateTime t)
{
    ParameterPlot* pp=new ParameterPlot;
    pp->setStartDate(t);
    pp->setParameter(p);

    m_parameters<<pp;
    connect(pp,SIGNAL(clicked()),this,SLOT(paramTrig()));
    m_parametersWidget->layout()->addWidget(pp);

}

void UnitOverview::printParameters(QList<Parameter *> pl,QDateTime start)
{
    for(int i=0;i<pl.count();i++)
        printParameter(pl[i],start);
}

void UnitOverview::printDevice(Device *p)
{
    DevicePlot* pp=new DevicePlot;
    pp->handle(p);
    m_devices<<pp;
    connect(pp,SIGNAL(clicked()),this,SLOT(deviceTrig()));

    m_devicesWidget->layout()->addWidget(pp);
}

void UnitOverview::printDevices(QList<Device *> pl)
{
    for(int i=0;i<pl.count();i++)
        printDevice(pl[i]);
}

void UnitOverview::printButtons(QStringList l)
{
    for(int i=0;i<l.count();i++)
    {
        ToolButton* b=new ToolButton(l[i]);
                connect(b,SIGNAL(clicked()),this,SLOT(buttonTrig()));
        m_buttonList<<b;
        m_buttons->layout()->addWidget(b);
    }
}

void UnitOverview::buttonTrig()
{
    ToolButton* t=dynamic_cast<ToolButton*>(sender());

    if(t && m_client)
        m_client->trigKey(t->title());
}

void UnitOverview::paramTrig()
{
    ParameterPlot* p=dynamic_cast<ParameterPlot*>(sender());
    if(p)
        emit requestParamEdit(p->parameter());
}

void UnitOverview::deviceTrig()
{
    DevicePlot* p=dynamic_cast<DevicePlot*>(sender());
    if(p)
        emit requestDeviceEdit(p->device());
}

void UnitOverview::dateChanged(QDateTime t)
{

}

ParameterPlot *UnitOverview::parameter() const
{
    return m_parameter;
}


void UnitOverview::refreshCentral()
{
    if(m_integral)
    {
        m_integral->initStyle();
        m_integral->updatePlot();
    }

    for(int i=0;i<m_parameters.count();i++)
        m_parameters[i]->refresh();
}

void UnitOverview::enableCentral(bool s)
{
    for(int i=0;i<m_devices.count();i++)
        m_devices[i]->setRefreshEnabled(s);

    if(m_integral)
    {
        m_integral->setEnabledPlot(s);
    }
}

void UnitOverview::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    m_integral->setMinimumSize(size()/2);
}




void UnitOverview::editParameter(Parameter *p)
{
    m_stack->setCurrentWidget(m_parameter);
    m_parameter->setParameter(p);
    m_parameter->setStartDate(m_client->startTime());
    m_device->handle(nullptr);
    enableCentral(false);
}

void UnitOverview::editDevice(Device *d)
{
    m_stack->setCurrentWidget(m_device);
    m_parameter->setParameter(nullptr);
    m_device->handle(d);
    enableCentral(false);
}

void UnitOverview::showCentral()
{
    enableCentral(true);

    m_stack->setCurrentWidget(m_overviewWidget);
    m_device->handle(nullptr);
    m_parameter->setParameter(nullptr);
}



ActuatorWidget::ActuatorWidget(QWidget *parent): QWidget(parent)
{
    setLayout(new QVBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(m_plot=new DevicePlot);
    layout()->addWidget(m_integralPlot=new IntegralPlot);

}

void ActuatorWidget::handle(Actuator *a)
{
    m_plot->handle(a);
    m_integralPlot->handle(a);
}

void ActuatorWidget::refresh()
{
    m_integralPlot->updatePlot();
}

void ActuatorWidget::enablePlot(bool s)
{

    qDebug()<<"enable to implement"<<s;
}
