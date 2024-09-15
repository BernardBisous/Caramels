#include "unitoverview.h"
#include "qboxlayout.h"
#include <QSplitter>
#define DETAIL_DEVICE false
#define DETAIL_PARAMETER false
UnitOverview::UnitOverview(QWidget *parent)
    : QWidget{parent},m_client(nullptr),m_integral(nullptr)
{
    setLayout(new QVBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    QWidget* top=new QWidget;
    top->setLayout(new QHBoxLayout);
    top->layout()->setContentsMargins(0,0,0,0);

    QWidget*mw=new QWidget;
    mw->setLayout(new QVBoxLayout);

    mw->layout()->addWidget(m_name=new QLabel);
    mw->layout()->addWidget(m_desc=new QLabel);
    top->layout()->addWidget(mw);
    mw->layout()->setContentsMargins(0,0,0,0);
    top->layout()->addWidget(m_buttons=new QWidget);
    top->layout()->addWidget(m_regulator=new RegulatorWidget);


    layout()->addWidget(top);


    top->layout()->setContentsMargins(0,0,0,0);
    m_desc->setWordWrap(true);
    m_desc->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_buttons->setLayout(new QHBoxLayout);
    m_buttons->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    m_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    top->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout()->addWidget(m_stack=new QStackedWidget);

    QSplitter*ovSplit=new QSplitter;
    m_overviewWidget=ovSplit;

    m_stack->addWidget(ovSplit);
    m_stack->addWidget(m_parameter=new ParameterPlot);
    m_stack->addWidget(m_device=new DevicePlot);


   // m_overviewWidget->setLayout(new QVBoxLayout);
    ovSplit->setOrientation(Qt::Vertical);
    QSplitter* wp=new QSplitter;
    m_paramSplitter=wp;
    //wp->setOrientation(Qt::Horizontal);
    //wp->layout()->setContentsMargins(0,0,0,0);
    wp->addWidget(m_integral=new IntegralPlot);
    wp->addWidget(m_parametersWidget=new QWidget);

    m_parametersWidget->setLayout(new QVBoxLayout);

    ovSplit->addWidget(wp);
    ovSplit->addWidget(m_devicesWidget=new QWidget);
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
    m_parametersWidget->layout()->addWidget(pp);

    pp->setStartDate(t);
    pp->setParameter(p);

    m_parameters<<pp;
    connect(pp,SIGNAL(clicked()),this,SLOT(paramTrig()));

}

void UnitOverview::printParameters(QList<Parameter *> pl,QDateTime start)
{
    for(int i=0;i<pl.count();i++)
        printParameter(pl[i],start);
}

void UnitOverview::printDevice(Device *p)
{
    DevicePlot* pp=new DevicePlot;
    m_devicesWidget->layout()->addWidget(pp);
    pp->handle(p);
    m_devices<<pp;
    connect(pp,SIGNAL(clicked()),this,SLOT(deviceTrig()));


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

void UnitOverview::computeLayout()
{
    m_overviewWidget->setStretchFactor(0,2);
    m_paramSplitter->setStretchFactor(0,1);

    m_overviewWidget->setStretchFactor(1,1);
    m_paramSplitter->setStretchFactor(1,1);

    m_parametersWidget->setHidden(m_parameters.isEmpty());
    m_devicesWidget->setHidden(m_devices.isEmpty());
            /*
    if(m_devices.isEmpty())
        m_overviewWidget->setStretchFactor(1,-1);
    else
         m_overviewWidget->setStretchFactor(1,1);

    if(m_parameters.isEmpty())
         m_paramSplitter->setStretchFactor(1,-1);
    else
         m_paramSplitter->setStretchFactor(1,1);
                 */

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

    //m_integral->setMinimumSize(size()/2);
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
    if(DETAIL_DEVICE)
    {
        m_stack->setCurrentWidget(m_device);
        m_device->handle(d);
        enableCentral(false);
    }

    m_parameter->setParameter(nullptr);

}

void UnitOverview::showCentral()
{
    enableCentral(true);

    m_stack->setCurrentWidget(m_overviewWidget);
    m_device->handle(nullptr);
    m_parameter->setParameter(nullptr);

    computeLayout();
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
