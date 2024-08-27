#ifndef HARDWAREOVERVIEW_H
#define HARDWAREOVERVIEW_H

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

};

#endif // HARDWAREOVERVIEW_H
