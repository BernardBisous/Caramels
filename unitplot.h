#ifndef UNITPLOT_H
#define UNITPLOT_H


#include "hardware/hardwareunit.h"
#include "qscatterseries.h"
#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QGridLayout>

#include <growconfig.h>
class UnitPlot  : public QWidget
{
     Q_OBJECT
public:
    UnitPlot(QWidget*parent=nullptr);
    void removeMargins();
    void handle(HardwareUnit* u,QDateTime t);
    void updateSensor(QLineSeries* l,Device*s);
    void updateParameter(QLineSeries* l, Parameter*p, QDateTime start);
    void initStyle();
    void setStartDate(const QDateTime &newStartDate);

private slots:
    void updatePlot();

private:
    QTimer* m_timer;
    QDateTime m_startDate;
    HardwareUnit* m_client;
    QChartView *m_view;
    QList<QLineSeries*> m_series;

    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;
    QChart *m_chart;
};

#endif // UNITPLOT_H
