#ifndef DEVICEPLOT_H
#define DEVICEPLOT_H

#include "hardware/device.h"
#include "qdatetime.h"
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QTimer>

class DevicePlot : public QChartView {
public:
    explicit DevicePlot(QWidget* parent = nullptr);
    void handle(Device*c);
    void setXrange(int ms);
    void initStyle();

    bool locked() const;
    void setLocked(bool newLocked);

public slots:

private:

    bool m_locked;
    Device* m_device;
    QChart* m_chart;
    QLineSeries* m_series;

    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;


    void updatePlot() ;


};

#endif // DEVICEPLOT_H
