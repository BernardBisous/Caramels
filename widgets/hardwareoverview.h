#ifndef HARDWAREOVERVIEW_H
#define HARDWAREOVERVIEW_H

#include "Interface/progresswidget.h"
#include "Interface/serialeditor.h"
#include "qgraphicsview.h"
#include "hardware/tent.h"
#include <QWidget>

class ChemicalInjectorWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ChemicalInjectorWidget(ChemicalInjector *i, QWidget* parent);
    void refresh();
private:
    ChemicalInjector* m_client;
    ProgressBar* m_bar;
    QLabel* m_name;
};

class TopConnect : public TopWidget
{
    Q_OBJECT
public:
    explicit  TopConnect(Tent* t,QWidget *parent = nullptr);

private:
    SerialEditor* m_editor;
};

class HardwareOverview : public QWidget
{
    Q_OBJECT
public:
    explicit HardwareOverview(QWidget *parent = nullptr);
    QGraphicsTextItem * printTextParameters(const QString &text, QPointF position);
    void handle(Tent* t);
    void showConnect(bool s);
    void addInjector(ChemicalInjector* t);
    void addDhts(QList<DHT>l);
    void updateDHTs();

public slots:
    void update();
    void connectedChanged(bool s);

private:
    QGraphicsScene *m_scene;
    Tent *m_tent;
    QTimer *m_timer;

    QGraphicsTextItem * m_heigh;
    QGraphicsTextItem * m_ph;
    QGraphicsTextItem * m_CO2;

    QGraphicsPixmapItem* m_sun;
    QGraphicsTextItem * m_spectrum;
    QGraphicsTextItem * m_lightsPower;
    QGraphicsRectItem * m_pumps;
    QGraphicsPixmapItem * m_tank;
    TopConnect* m_connect;

    QGraphicsView* m_view;

    QList<ChemicalInjectorWidget*> m_injectors;
    QList<DHT> m_dhts;
    QList<QGraphicsTextItem*> m_dhtItems;

};

#endif // HARDWAREOVERVIEW_H
