#ifndef HARDWAREOVERVIEW_H
#define HARDWAREOVERVIEW_H

#include "Interface/menubutton.h"
#include "qgraphicsview.h"
#include "tent.h"
#include <QWidget>

class HardwareOverview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HardwareOverview(QWidget *parent = nullptr);
    QGraphicsTextItem * printTextParameters(const QString &text, QPointF position);
    void handle(Tent* t);

public slots:
    void update();

private:
    QGraphicsScene *m_scene;
    Tent *m_tent;
    QTimer *m_timer;

    QGraphicsTextItem * m_ph;
    QGraphicsTextItem * m_waterTemp;
    QGraphicsTextItem * m_CO2;
    QGraphicsTextItem * m_humidity;
    QGraphicsTextItem * m_airTemp;
    QGraphicsPixmapItem* m_sun;
    QGraphicsTextItem * m_spectrum;
    QGraphicsTextItem * m_pumps;


};

#endif // HARDWAREOVERVIEW_H
