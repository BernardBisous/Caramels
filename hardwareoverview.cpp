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
    QPixmap plantPixmap("path/to/your/plant.png");
    m_scene->addPixmap(plantPixmap);

    connect(m_timer, &QTimer::timeout, this, &HardwareOverview::update);
    m_timer->start(1000); // Update every second
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
