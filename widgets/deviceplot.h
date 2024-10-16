#ifndef DEVICEPLOT_H
#define DEVICEPLOT_H

#include "hardware/device.h"
#include "qdatetime.h"
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QTimer>
#include <QGraphicsLayout>


#include "Interface/slider.h"

class DevicePlot : public QChartView
{
    Q_OBJECT
public:
    explicit DevicePlot(QWidget* parent = nullptr);
    void handle(Device*c);
    void setXrange(int ms);
    void initStyle();
    void setRefreshEnabled(bool s);
    void setXRangeSecs(int newXRangeS);
    void updateScale();
    void setSliderValue(int i);
    void setRealTime(bool s);
    void mousePressEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton) {
                // Emit the clicked signal
                emit clicked();
            }
            QWidget::mousePressEvent(event);
        }

    Device *device() const;

public slots:
     void updatePlot() ;
     void sliderChanged(int val);

signals:
    void clicked();

private:

    Device* m_device;
    QChart* m_chart;
    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;
    QLineSeries* m_series;
    int m_xRangeSecs;
    QDateTime m_minDate;
    Slider* m_slider;



};

#endif // DEVICEPLOT_H
