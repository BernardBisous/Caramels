#include "hardwareoverview.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"

#define AUTOPOLL false

HardwareOverview::HardwareOverview(QWidget *parent)
    :
      QGraphicsView(parent),
      m_scene(new QGraphicsScene()),
      m_tent(nullptr),
      m_timer(new QTimer(this))
{
    setScene(m_scene);

            // Load plant image as background
    QPixmap plantPixmap(":/icons/plant");
    m_scene->addPixmap(plantPixmap.scaledToHeight(400));

    connect(m_timer, &QTimer::timeout, this, &HardwareOverview::update);

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

    update();
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
