#include "serialeditor.h"
#include "qboxlayout.h"

SerialEditor::SerialEditor(QWidget *parent)
    : QWidget{parent},m_client(nullptr)
{
    setLayout(new QVBoxLayout);

    layout()->addWidget(m_ports=new MenuButton);
     m_ports->setPrintActive(false);
     connect(m_ports,SIGNAL(actionTrigg(QString)),this,SLOT(portSlot(QString)));

}

void SerialEditor::handle(Tent *t)
{
    m_client=t;
    m_ports->clear();
    m_ports->addActions(t->availablePorts());

    if(t->connected())
    {
        m_ports->setText("✅ "+t->serialPort());
    }
    else
    {
        m_ports->setText("Select port");

    }
}

void SerialEditor::refresh()
{

}

void SerialEditor::connectedChanged()
{

}

void SerialEditor::portSlot(QString s)
{
    m_client->setSerialPort(s);
}
