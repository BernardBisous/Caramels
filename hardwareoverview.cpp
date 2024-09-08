#include "hardwareoverview.h"
#include "qboxlayout.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qlabel.h"

#define AUTOPOLL false

HardwareOverview::HardwareOverview(QWidget *parent)
    :
      QGraphicsView(parent),
      m_scene(new QGraphicsScene()),
      m_tent(nullptr),
      m_timer(new QTimer(this)),m_connect(nullptr)
{
    setScene(m_scene);

            // Load plant image as background
    QPixmap plantPixmap(":/icons/plant");
    m_scene->addPixmap(plantPixmap.scaledToHeight(400));

    connect(m_timer, &QTimer::timeout, this, &HardwareOverview::update);

    m_pumps=printTextParameters("Stock d'eau:",QPointF(20,320));

    m_ph=printTextParameters("PH:",QPointF(20,280));
    m_waterTemp=printTextParameters("Temp:",QPointF(80,280));

    m_CO2=printTextParameters("CO2:",QPointF(10,110));
    m_humidity=printTextParameters("Humidity:",QPointF(10,160));
    m_airTemp=printTextParameters("Temperature:",QPointF(10,210));


    m_sun=m_scene->addPixmap(QPixmap(":/icons/sun").scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    m_sun->setPos(QPointF(10,-80));
    m_spectrum=printTextParameters("Color:",QPointF(70,-60));


    if(AUTOPOLL)
        m_timer->start(1000); // Update every second



}

QGraphicsTextItem * HardwareOverview::printTextParameters(const QString &text, QPointF position)
{
    QFont f=font();

    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
    textItem->setPos(position);
    auto a=m_scene->addText(text);
    a->setParentItem(textItem);
    a->setPos(QPointF(0,-15));


    f.setPointSize(14);
    textItem->setFont(f);
    m_scene->addItem(textItem);
    return textItem;
}

void HardwareOverview::handle(Tent *t)
{
    m_tent=t;

    if(!AUTOPOLL)
        connect(t,SIGNAL(sensorsAquiered()),this,SLOT(update()));

    connect(t,SIGNAL(connectedHardware(bool)),this,SLOT(connectedChanged(bool)));


    showConnect(t->connected());
    update();
}

void HardwareOverview::showConnect(bool s)
{
    qDebug()<<"Show connect serial"<<s;


    if(s && !m_connect)
        return;

    if(!s && !m_connect)
    {
        m_connect=new TopConnect(m_tent,this);
        return;
    }

    if(!s)
           m_connect->start();
    else
           m_connect->stop();
}

void HardwareOverview::update()
{

    if(!m_tent)
        return;


    m_ph->setPlainText(QString::number(m_tent->PH(),'f',1));
    m_waterTemp->setPlainText(QString::number(m_tent->temperature(0),'f',1)+"°c");
    m_airTemp->setPlainText(QString::number(m_tent->temperature(1),'f',1)+"°c");
    m_humidity->setPlainText(QString::number(m_tent->humidity(),'f',1)+"%");
    m_CO2->setPlainText(QString::number(m_tent->CO2(),'f',1)+"ppm");

    QString sp=m_tent->injectingState();
    if(sp.isEmpty())
        m_pumps->hide();
    else
        m_pumps->show();
    m_pumps->setPlainText(sp);

    if(m_tent->lightPower()==0)
    {
        m_sun->hide();
        m_spectrum->hide();
    }
    else {
        m_sun->show();
        m_spectrum->show();
        m_spectrum->setPlainText(QString::number(m_tent->lightSpectrum(),'f',1)+"nm");
    }

}

void HardwareOverview::connectedChanged(bool s)
{
    showConnect(s);
}

TopConnect::TopConnect(Tent *t, QWidget *parent):TopWidget(parent)
{
    m_central->setLayout(new QVBoxLayout);
    m_central->layout()->addWidget(new QLabel("Electronique introuvable"));
    m_central->layout()->addWidget(m_editor=new SerialEditor);

    connect(t,SIGNAL(connectedHardware(bool)),this,SLOT(trigSlot(bool)));
    m_editor->handle(t);
    start();
}

void TopConnect::trigSlot(bool s)
{

}




