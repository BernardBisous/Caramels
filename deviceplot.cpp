#include "deviceplot.h"
#include <QGraphicsLayout>

DevicePlot::DevicePlot(QWidget *parent):
    QChartView(parent), m_locked(true),
    m_device(nullptr), m_chart(new QChart()),m_series(new QLineSeries())

{


    setContentsMargins(0,0,0,0);
    // Set up the chart

   // m_chart->setTitle("Device Historical Data");




    m_chart->legend()->setVisible(false);
    setChart(m_chart);
    m_chart->layout()->setContentsMargins(0,0,0,0);


    // Create the axes
    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;

    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    // Add the series to the chart





    initStyle();
 //   updatePlot();

    setRenderHint(QPainter::Antialiasing);



}

void DevicePlot::handle(Device *c)
{
    if(m_device)
        disconnect(m_device, &Device::newValue, this, &DevicePlot::updatePlot);

    m_device=c;

    if(!c)
        return;


     m_yAxis->setTitleText("Value ["+c->units()+"]");

    connect(m_device, &Device::newValue, this, &DevicePlot::updatePlot);

    initStyle();
    updatePlot();


}

void DevicePlot::initStyle()
{
    QColor gridColor(palette().alternateBase().color());
    QColor textColor(palette().text().color());

    m_chart->setBackgroundBrush(palette().base());
    m_series->setColor(palette().highlight().color());
    m_chart->setPlotAreaBackgroundPen(QPen(Qt::yellow));

    m_xAxis->setGridLineColor(gridColor);
    m_xAxis->setLinePenColor(gridColor);
    m_xAxis->setLabelsColor(textColor);
    m_xAxis->setTitleBrush(textColor);

    m_yAxis->setGridLineColor(gridColor);
    m_yAxis->setLinePenColor(gridColor);
    m_yAxis->setLabelsColor(textColor);
    m_yAxis->setTitleBrush(textColor);
    m_chart->setBackgroundRoundness(20);

    m_chart->setTitleBrush(textColor);
    m_chart->legend()->hide();
    m_chart->update();
}



bool DevicePlot::locked() const
{
    return m_locked;
}

void DevicePlot::setLocked(bool newLocked)
{
    m_locked = newLocked;
}

void DevicePlot::updatePlot()
{
    chart()->removeSeries(m_series);
    m_series->clear();

    if(!m_device)
        return;

    auto l=m_device->values();
    auto lh=m_device->historic();

    QDateTime sh;
    if(!l.isEmpty())
        sh=l.first().time;

    float max=0;
    QDateTime minDate;
    if(!lh.isEmpty())
        minDate=lh.first().time;
    else if(!l.isEmpty())
        minDate=l.first().time;
    else
        minDate=QDateTime::currentDateTime().addSecs(-10);

    for(int i=0;i<lh.count();i++)
    {
        if(sh.isValid() && lh[i].time<sh)
        {
            if(!locked() || lh[i].time>minDate)
            {
                if(!i || max<lh[i].value)
                    max=lh[i].value;

                m_series->append(lh[i].time.toMSecsSinceEpoch(),lh[i].value);
            }
        }
    }

    for(int i=0;i<l.count();i++)
    {
        if(!locked() || l[i].time>minDate)
        {
            if((!i&&lh.isEmpty()) || max<l[i].value)
                max=l[i].value;


            m_series->append(l[i].time.toMSecsSinceEpoch(),l[i].value);
        }
    }





    if(m_series->count()<=1)
    {
       m_series->append(minDate.toMSecsSinceEpoch(),m_device->currentValue());
    }

    m_series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),m_device->currentValue());

    m_chart->addSeries(m_series);

    m_xAxis->setRange(minDate,QDateTime::currentDateTime().addSecs(1));
    m_yAxis->setMax(max*1.1);
    m_yAxis->setMin(0);

    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);
}
