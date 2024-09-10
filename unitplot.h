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
    void handle(HardwareUnit* u);
    void updateScale();
    void updateSensor(QLineSeries* l,Device*s);
    void updateParameter(QLineSeries* l, Parameter*p, QDateTime start);
    void initStyle();
    float maxY();
    void setStartDate(const QDateTime &newStartDate);
    QLineSeries* paramSerie(Parameter*p);

    QDateTime endConfig();
    void mousePressEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton) {
                // Emit the clicked signal
                emit clicked();
            }
            QWidget::mousePressEvent(event);
        }

    HardwareUnit *client() const;

    Device *targeting() const;
    void setTargeting(Device *newTargeting);


public slots:
    void updatePlot();
    void updateSensors();
    void updateParameters();


signals:
    void clicked();

private:
    QTimer* m_timer;
    QDateTime m_startDate;
    HardwareUnit* m_client;
    QChartView *m_view;
    QList<QLineSeries*> m_devicesSeries;
    QList<QLineSeries*> m_parametersSeries;
    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;
    QChart *m_chart;
    Device* m_targeting;
};

#endif // UNITPLOT_H
