#include "deviceeditor.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qregularexpression.h"

DeviceEditor::DeviceEditor(QWidget *parent)
    : ActionWidget{parent},m_client(nullptr),m_abstracted(false)
{
    setLayout(new QVBoxLayout);

    setMode(noBorder);
   // setHoverable(false);





    layout()->addWidget(m_nameLabel=new QLabel);
    layout()->addWidget(m_status=new QLabel);


    layout()->addWidget(m_valueSlider=new Slider);



    m_dataWidget=new QWidget;
    m_dataWidget->setLayout(m_dataLAyout=new QFormLayout);
  //  m_dataWidget->layout()->setContentsMargins(0,0,0,0);
  //  m_dataWidget->layout()->addWidget(new QLabel("Parameters:"));
  //  QWidget* fw=new QWidget;
  //  fw->setLayout(m_dataLAyout=new QFormLayout);
  //  m_dataWidget->layout()->addWidget(fw);
    m_dataLAyout->setContentsMargins(0,0,0,0);


    m_resultWidget=new QWidget;
    m_resultWidget->setLayout(m_resultLayout=new QFormLayout);
  //  m_resultWidget->layout()->setContentsMargins(0,0,0,0);
 //   m_resultWidget->layout()->addWidget(new QLabel("Results:"));
 //   QWidget* rw=new QWidget;
 //   rw->setLayout(m_resultLayout=new QFormLayout);
//    m_resultWidget->layout()->addWidget(rw);
    m_resultLayout->setContentsMargins(0,0,0,0);


    layout()->addWidget(m_resultWidget);
    layout()->addWidget(m_dataWidget);

    QFont f=font();
    f.setPointSize(f.pointSize()+4);
    m_nameLabel->setFont(f);

    connect(m_valueSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderEdited(int)));
    m_valueSlider->setOrientation(Qt::Horizontal);

}

void DeviceEditor::handle(Device *c)
{

    if(m_client)
    {
         disconnect(c,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    }
    m_dataFields.clear();
    clearField(m_dataLAyout);
    clearField(m_resultLayout);

    m_client=c;

    if(!m_client)
        return;

    connect(c,SIGNAL(newValue(float)),this,SLOT(valueSlot(float)));
    m_valueSlider->setEnabled(dynamic_cast<Actuator*>(m_client));

    auto l=m_client->dataKeys();

    m_dataWidget->setHidden(l.isEmpty() || m_abstracted);
    for(int i=0;i<l.count();i++)
    {

        QLineEdit* el=new QLineEdit(this);
        el->setText(m_client->dataValue(l[i]));
        m_dataLAyout->addRow(l[i],el);
        m_dataFields.append(el);
        connect(el, SIGNAL(editingFinished()),this,SLOT(editSlot()));
    }




    auto lc=m_client->resultKeys();
    m_resultWidget->setHidden(lc.isEmpty() || m_abstracted);

    for(int i=0;i<lc.count();i++)
    {
        QLabel* la=new QLabel(this);
        QString sl=lc[i];
        QString units;

        QRegularExpression regex(R"((\w+)(\[(\w+)\])?)");
        QRegularExpressionMatch match = regex.match(sl);
        if(match.hasMatch()) {
           sl = match.captured(1);
           units = match.captured(3);
        }


        la->setText(QString::number(m_client->computeResult(lc[i])) +units);
        m_resultLayout->addRow(sl+":",la);

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


    valueSlot(m_client->currentValue());


}

void DeviceEditor::setAbstracted(bool s)
{
    m_abstracted=s;
    m_valueSlider->setHidden(s);
    m_dataWidget->setHidden(s);
}

void DeviceEditor::clearField(QFormLayout *f)
{

    // Possible
    while(f->count())
    {
        auto i=f->takeAt(0);

        if(i->widget())
            delete i->widget();

        delete i;
    }
}



void DeviceEditor::editSlot()
{
    QObject* o=sender();


    for(int i=0;i<m_dataFields.count();i++)
    {
        if(m_dataFields[i]==o)
        {

            QLineEdit* el=dynamic_cast<QLineEdit*>(o);
            if(el)
            {
                QString sn=m_client->dataKeys()[i];
                m_client->setDataValue(sn,el->text(),true);

                emit edited(sn,el->text());
            }
        }
    }
}

void DeviceEditor::sliderEdited(int i)
{
    Actuator*a=static_cast<Actuator*>(m_client);
    a->applyPurcent(i);
}

void DeviceEditor::valueSlot(float t)
{
    m_status->setText("Value: "+QString::number(t));

    disconnect(m_valueSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderEdited(int)));
    m_valueSlider->setValue(t);
    connect(m_valueSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderEdited(int)));
}

Device *DeviceEditor::client() const
{
    return m_client;
}
