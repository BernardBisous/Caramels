#include "parametereditor.h"

ParameterEditor::ParameterEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{

    setLayout(new QHBoxLayout);

    layout()->addWidget(m_plot=new ParameterPlot);

    m_editWidget=new QWidget;
    m_editWidget->setLayout(new QVBoxLayout);
    m_editWidget->layout()->addWidget(m_name=new NameLabel);
    m_editWidget->layout()->addWidget(m_desc=new QLabel);

     m_editWidget->layout()->addWidget(m_deviceStatus=new QLabel("\nHardware associé:"));
    m_editWidget->layout()->addWidget(m_deviceArea=new ScrollArea);

    layout()->addWidget(m_editWidget);

    m_desc->setWordWrap(true);

    m_editWidget->setFixedWidth(250);
    connect(m_plot,SIGNAL(clicked(QPointF)),this,SLOT(pointClickedSlot(QPointF)));
    connect(m_name,SIGNAL(edited(QString)),this,SLOT(nameChangedSlot(QString)));

}

void ParameterEditor::handle(Parameter *p)
{


    m_client=p;

    if(!p)
        return;

    refresh();



    m_deviceArea->clear();
    auto l=p->devices();

    m_deviceStatus->setHidden(l.isEmpty());
    for(int i=0;i<l.count();i++)
    {
        DeviceEditor* de=new DeviceEditor;
        de->setHoverable(true);
        de->setMode(ActionWidget::appearingBorder);
        de->setAbstracted(true);
        m_deviceArea->addWidget(de);
        de->handle(l[i]);

        connect(de,SIGNAL(clicked()),this,SLOT(deviceSlot()));
    }
    m_deviceArea->addSpacer();

}

void ParameterEditor::setXRange(int t)
{
    m_plot->setXrange(t);
}

void ParameterEditor::refresh()
{
    if(!m_client)
        return;

    m_plot->refresh(m_client);
    m_name->load(m_client->name());
    m_desc->setText(m_client->description());


}

void ParameterEditor::deviceSlot()
{

    auto a=dynamic_cast<DeviceEditor*>(sender());
    if(a && a->client())
    {
        qDebug()<<"Switch to Edit devce"<<a->client()->name();
        emit editDevice(a->client());
    }
}

void ParameterEditor::nameChangedSlot(QString s)
{
    m_client->setName(s);
}

void ParameterEditor::pointClickedSlot(QPointF p)
{

    TimedValue v;
    v.hourIndex=p.x();
    v.value=p.y();

    int i=m_client->append(v);

    m_plot->refresh(m_client);
    m_plot->select(i);

}
