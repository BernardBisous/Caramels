#ifndef UNITPLOT_H
#define UNITPLOT_H


#include "Interface/slider.h"
#include "hardware/hardwareunit.h"
#include "qscatterseries.h"
#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QGridLayout>

class UnitPlot  : public QWidget
{
     Q_OBJECT
public:
    UnitPlot(QWidget*parent=nullptr);
    void removeMargins();
    void enable(bool s);





    void handle(HardwareUnit* u, QList<Device*> devices=QList<Device*>(),QList<Parameter*>parameters=QList<Parameter*> ());
    void updateScale();
    void updateSensor(QLineSeries* l,Device*s);
    void updateParameter(QLineSeries* l, Parameter*p, QDateTime start);
    void initStyle();
    float maxY();
    void setStartDate(const QDateTime &newStartDate);
    QLineSeries* paramSerie(Parameter*p);
    QLineSeries* deviceSerie(Device*d);
    void initSeriesParemeters();
    void initSeriesDevices();


    void hideAll(bool s);

    void highlight(Parameter*p);
    void highlight(Device*d);
    void highlight(QList<QLineSeries*> l);
    void updateHighlight();

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


    QList<Device *> devices() const;
    void setDevices(const QList<Device *> &newDevices);

    QList<Parameter *> parameters() const;
    void setParameters(const QList<Parameter *> &newParameters);

    Device* attachedDevice(Parameter*p);

public slots:
    void updatePlot();
    void updateSensors();
    void updateParameters();
    void sliderChanged(int i);


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

    Slider*m_rangeSlider;
    int m_xRangeMs;

    Device* m_highlightedDevice;
    Parameter* m_highlightedParameter;

    QList<Device*> m_devices;
    QList<Parameter*> m_parameters;


};

#endif // UNITPLOT_H
