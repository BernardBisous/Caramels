#include "deviceplot.h"
#include <QGraphicsLayout>

DevicePlot::DevicePlot(QWidget *parent):
    QChartView(parent), m_device(nullptr),
    m_chart(new QChart()), m_series(new QLineSeries())

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
    m_yAxis->setTitleText("Value");
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    // Add the series to the chart

    m_chart->addSeries(m_series);
    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);



    initStyle();


}

void DevicePlot::handle(Device *c)
{

    m_device=c;

    if(!c)
        return;



    connect(m_device, &Device::newValue, this, &DevicePlot::updatePlot);

   // initStyle();
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
}

void DevicePlot::updatePlot()
{
    chart()->removeSeries(m_series);
    m_series->clear();

    auto l=m_device->historic();
    for(int i=0;i<l.count();i++)
    {
        m_series->append(l[i].time.toMSecsSinceEpoch(),l[i].value);
    }


    m_chart->addSeries(m_series);

    //chart->axisX()->rescale();
    //chart->axisY()->rescale();
}
