#include "regulatorwidget.h"
#include "qboxlayout.h"

RegulatorWidget::RegulatorWidget(QWidget *parent)
    : ActionWidget{parent},m_client(nullptr),m_regulator(nullptr)
{

    setHoverable(false);
    setMode(noBorder);
    setLayout(new QHBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(20);

    QWidget*v=new QWidget;
    v->setLayout(new QVBoxLayout);

    v->layout()->addWidget(new QLabel("Next regulation:"));
    v->layout()->addWidget(m_regLabel=new QLabel);


    QWidget*z=new QWidget;
    z->setLayout(new QVBoxLayout);

    z->layout()->addWidget(new QLabel("Value:"));
    z->layout()->addWidget(m_valueLabel=new QLabel);



    layout()->addWidget(z);
    layout()->addWidget(v);
    layout()->addWidget(m_button=new ToolButton("Regulate"));

    QFont f=font();
    f.setPointSize(16);
    m_regLabel->setFont(f);
    m_valueLabel->setFont(f);

    hide();

    connect(m_button,SIGNAL(clicked()),this,SLOT(buttonSlot()));
    refresh();
}

void RegulatorWidget::handle(AnalogSensor *a)
{



    m_client=a;



    if(m_client)
    {
        connect(m_client,SIGNAL(regulated()),this,SLOT(refresh()));
        connect(m_client,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    }

    m_regulator=nullptr;
    refresh();
}

void RegulatorWidget::reset()
{
    if(m_client)
    {
        disconnect(m_client,SIGNAL(regulated()),this,SLOT(refresh()));
        disconnect(m_client,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
        m_client=nullptr;
    }

    m_regulator=nullptr;

    if(m_regulator && m_regulator->device())
        disconnect(m_regulator->device(),SIGNAL(newValue(float)),this,SLOT(refresh()));

}


void RegulatorWidget::setRegulator(RegulatingTimer *newRegulator)
{

    if(m_regulator && m_regulator->device())
        disconnect(m_regulator->device(),SIGNAL(newValue(float)),this,SLOT(refresh()));

    m_button->setText("Switch");
    m_regulator = newRegulator;

    setHidden(!m_regulator);

    if(m_regulator && m_regulator->device())
        connect(m_regulator->device(),SIGNAL(newValue(float)),this,SLOT(refresh()));


    refresh();
}


void RegulatorWidget::refresh()
{

    setHidden(!m_client && !m_regulator);

    if(m_client)
    {
        QDateTime t=m_client->nextRegulation();
        if(t.isValid())
        {
            m_regLabel->setText(m_client->nextRegulation().toString("hh:mm"));
        }
        else
            m_regLabel->setText("Soudain");


        m_valueLabel->setText(m_client->userValue());
    }



    else if(m_regulator)
    {
        m_valueLabel->setText(m_regulator->currentString());
        m_regLabel->setText(m_regulator->nextSwitch().toString("hh:mm"));
    }
}

void RegulatorWidget::valueSlot(float )
{
    refresh();
}

void RegulatorWidget::buttonSlot()
{
    if(m_client)
    {
        m_client->regulateNow();

    }
    if(m_regulator)
    {
        m_regulator->userSwitch();
    }
    refresh();
}

RegulatingTimer *RegulatorWidget::regulator() const
{
    return m_regulator;
}


