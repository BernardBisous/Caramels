#ifndef INTEGRALPLOT_H
#define INTEGRALPLOT_H

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QGraphicsLayout>
#include "hardware/device.h"

class IntegralPlot : public QChartView
{
    Q_OBJECT
public:
    explicit IntegralPlot(QWidget *parent = nullptr);
    void handle(Actuator*c);
    void handle( QList<Actuator*>s);
    void attach(Actuator*c);
    void initStyle();
    void removeAll();
    void updateSeries(Actuator*c,QLineSeries*s);
    void setEnabledPlot(bool s);

public slots:
    void updatePlot() ;
    void timerSlot();

signals:

private:
    QList<Actuator*> m_devices;
    QChart* m_chart;
    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;
    QList<QLineSeries*> m_series;

    QTimer* m_timer;

    QDateTime minDate;
    float min;
    float max;
};



#endif // INTEGRALPLOT_H
