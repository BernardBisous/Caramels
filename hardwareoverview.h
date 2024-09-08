#ifndef HARDWAREOVERVIEW_H
#define HARDWAREOVERVIEW_H

#include "Interface/menubutton.h"
#include "Interface/progresswidget.h"
#include "Interface/serialeditor.h"
#include "qgraphicsview.h"
#include "tent.h"
#include <QWidget>

class TopConnect : public TopWidget
{
    Q_OBJECT
public:
    explicit  TopConnect(Tent* t,QWidget *parent = nullptr);

public slots:
    void trigSlot(bool s);


private:

    SerialEditor* m_editor;
};

class HardwareOverview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HardwareOverview(QWidget *parent = nullptr);
    QGraphicsTextItem * printTextParameters(const QString &text, QPointF position);
    void handle(Tent* t);
    void showConnect(bool s);

public slots:
    void update();
    void connectedChanged(bool s);

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

    TopConnect* m_connect;

};

#endif // HARDWAREOVERVIEW_H
