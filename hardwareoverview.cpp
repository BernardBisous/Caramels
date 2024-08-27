#include "hardwareoverview.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"

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
    m_scene->addPixmap(plantPixmap.scaledToHeight(300));

    connect(m_timer, &QTimer::timeout, this, &HardwareOverview::update);
    m_timer->start(1000); // Update every second




    QPen p(palette().highlight().color());

    QLineF l1(-100,4,100,10);
    m_scene->addLine(l1,p);

    QRectF rpot(-50,420,150,50);
    m_scene->addRect(rpot,p);

    rpot.setTop(300);
    rpot.setBottom(350);
    m_scene->addRect(rpot,p);
}

QGraphicsTextItem * HardwareOverview::printTextParameters(const QString &text, QPointF position)
{
    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
    textItem->setPos(position);
    m_scene->addItem(textItem);
    return textItem;
}

void HardwareOverview::handle(Tent *t)
{
    m_tent=t;
}

void HardwareOverview::update()
{

}
