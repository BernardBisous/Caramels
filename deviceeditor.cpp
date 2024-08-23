#include "deviceeditor.h"
#include "qboxlayout.h"
#include "qlineedit.h"

DeviceEditor::DeviceEditor(QWidget *parent)
    : ActionWidget{parent},m_client(nullptr),m_abstracted(false)
{
    setLayout(new QVBoxLayout);

    setMode(noBorder);
    setHoverable(false);





    layout()->addWidget(m_nameLabel=new QLabel);
    layout()->addWidget(m_status=new QLabel);


    layout()->addWidget(m_valueSlider=new Slider);


    /*
    m_resultsWidget=new QWidget;
    m_resultsWidget->setLayout(m_resultsLayout=new QFormLayout);
    m_resultsLayout->setContentsMargins(0,0,0,0);
    */

    m_dataWidget=new QWidget;
    m_dataWidget->setLayout(m_dataLAyout=new QFormLayout);
    m_dataLAyout->setContentsMargins(0,0,0,0);

  //  layout()->addWidget(m_resultsWidget);
    layout()->addWidget(m_dataWidget);

    QFont f=font();
    f.setPointSize(f.pointSize()+4);
    m_nameLabel->setFont(f);

    connect(m_valueSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderEdited(int)));
    m_valueSlider->setOrientation(Qt::Horizontal);

}

void DeviceEditor::handle(Device *c)
{
    m_client=c;

    while(m_dataLAyout->count())
    {
        auto i=m_dataLAyout->takeAt(0);

        if(i->widget())
            delete i->widget();

        delete i;
    }



    if(!m_client)
        return;

    connect(c,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    m_valueSlider->setEnabled(dynamic_cast<Actuator*>(m_client));



    auto l=m_client->dataKeys();
    m_dataWidget->setHidden(l.isEmpty() || m_abstracted);

    for(int i=0;i<l.count();i++)
    {
        QLineEdit* el=new QLineEdit(this);
        el->setPalette(palette());

        el->setAutoFillBackground(false);

        el->setText(m_client->dataValue(l[i]));
        m_dataLAyout->addRow(l[i],el);
        connect(el, SIGNAL(editingFinished()),this,SLOT(editSlot()));
    }

    refresh();
}

QBrush DeviceEditor::backgroundBrush()
{
    return palette().base();
}

void DeviceEditor::refresh()
{
    m_nameLabel->setText(m_client->name());

    disconnect(m_valueSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderEdited(int)));
    m_valueSlider->setValue(m_client->currentValue());
    connect(m_valueSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderEdited(int)));

    valueSlot(m_client->currentValue());


}

void DeviceEditor::setAbstracted(bool s)
{
    m_abstracted=s;
    m_valueSlider->setHidden(s);
    m_dataWidget->setHidden(s);
}

void DeviceEditor::editSlot()
{
    QObject* o=sender();
    for(int i=0;i<m_dataLAyout->count();i++)
    {
        if(m_dataLAyout->itemAt(i)->widget()==o)
        {
            QLineEdit* el=dynamic_cast<QLineEdit*>(o);
            if(el)
            {
                qDebug()<<"edited meta"<<m_client->dataKeys()[i]<<el->text();
                emit edited(m_client->dataKeys()[i],el->text());
            }

        }
    }
    {
        auto i=m_dataLAyout->takeAt(0);

        if(i->widget())
            delete i->widget();

        delete i;
    }
}

void DeviceEditor::sliderEdited(int i)
{
    m_client->applyPurcent(i);
}

void DeviceEditor::valueSlot(float t)
{
    m_status->setText("Value:"+QString::number(t));

}

Device *DeviceEditor::client() const
{
    return m_client;
}
