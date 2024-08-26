#include "deviceplot.h"
#include <QGraphicsLayout>

DevicePlot::DevicePlot(QWidget *parent):
    QChartView(parent), m_device(nullptr),
    m_chart(new QChart()), m_series(new QLineSeries()),m_locked(true)

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
    m_yAxis->setTitleText("Value [%]");
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    // Add the series to the chart





    initStyle();
 //   updatePlot();



}

void DevicePlot::handle(Device *c)
{
    if(m_device)
        disconnect(m_device, &Device::newValue, this, &DevicePlot::updatePlot);

    m_device=c;

    if(!c)
        return;



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


    QDateTime min=QDateTime::currentDateTime().addSecs(-10);

    auto l=m_device->historic();





   // QDateTime max=QDateTime::currentDateTime();

    float maxY=100;
    float minY=0;

    for(int i=0;i<l.count();i++)
    {
        if(!locked() || l[i].time>min)
        {


            if(l[i].value>maxY)
                maxY=l[i].value;

            if(!i || l[i].value<minY)
                minY=l[i].value;


            m_series->append(l[i].time.toMSecsSinceEpoch(),l[i].value);
        }


    }


    if(m_series->count()<=1)
    {
        m_series->append(min.toMSecsSinceEpoch(),m_device->currentValue());
    }

    m_series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),m_device->currentValue());




     m_yAxis->setMax(maxY+5);
     m_yAxis->setMin(minY-5);


    m_chart->addSeries(m_series);
    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);


  //  m_yAxis->applyNiceNumbers();
    m_xAxis->setRange(min,QDateTime::currentDateTime());





}
