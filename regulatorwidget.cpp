#include "regulatorwidget.h"
#include "qboxlayout.h"

RegulatorWidget::RegulatorWidget(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{
    setLayout(new QHBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(20);

    QWidget*v=new QWidget;
    v->setLayout(new QVBoxLayout);
    v->layout()->setContentsMargins(0,0,0,0);
    v->layout()->addWidget(new QLabel("Next regulation:"));
    v->layout()->addWidget(m_regLabel=new QLabel);


    QWidget*z=new QWidget;
    z->setLayout(new QVBoxLayout);
    z->layout()->setContentsMargins(0,0,0,0);
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

    if(m_client)
    {
        disconnect(m_client,SIGNAL(regulated()),this,SLOT(refresh()));
        disconnect(m_client,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    }

    m_client=a;
    if(m_client)
    {
        connect(m_client,SIGNAL(regulated()),this,SLOT(refresh()));
        connect(m_client,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    }

    setHidden(!m_client);
    refresh();
}

void RegulatorWidget::refresh()
{

    if(!m_client)
        return;

    QDateTime t=m_client->nextRegulation();
    if(t.isValid())
    {
        m_regLabel->setText(m_client->nextRegulation().toString("hh:mm"));
    }
    else
        m_regLabel->setText("Soudain");


    m_valueLabel->setText(m_client->userValue());
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
        refresh();
    }
}

