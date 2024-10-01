#include "deviceplot.h"

DevicePlot::DevicePlot(QWidget *parent):
    QChartView(parent),
    m_device(nullptr), m_chart(new QChart()),m_series(new QLineSeries()),m_xRangeSecs(10)

{
    setContentsMargins(0,0,0,0);

    m_chart->legend()->setVisible(false);
    setChart(m_chart);
    m_chart->layout()->setContentsMargins(0,0,0,0);

    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;

    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignRight);


    m_chart->addSeries(m_series);

    setMinimumSize(100,100);


    m_yAxis->setTitleVisible(false);
    m_xAxis->setVisible(false);

    initStyle();
    setRenderHint(QPainter::Antialiasing);

    m_slider=new Slider(this);

    QRect re;
    re.setHeight(20);
    re.setWidth(100);
    re.translate(20,20);
    m_slider->setGeometry(re);
    connect(m_slider,SIGNAL(valueChanged(int)),this,SLOT(sliderChanged(int)));
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
     m_yAxis->setVisible(m_device->continousStreaming());
    updatePlot();
    m_slider->setValue(50);


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

void DevicePlot::setRefreshEnabled(bool s)
{
    if(!m_device)
        return;
    if(!s)
        disconnect(m_device, &Device::newValue, this, &DevicePlot::updatePlot);

    else
        connect(m_device, &Device::newValue, this, &DevicePlot::updatePlot);

}





void DevicePlot::updatePlot()
{
    if(m_chart->series().contains(m_series))
        m_chart->removeSeries(m_series);

    m_series->clear();

    if(!m_device)
        return;



    auto lh=m_device->historic();

    float lastVal=0;

    QDateTime minDate;
    if(!lh.isEmpty())
        minDate=lh.first().time;
    else
        minDate=QDateTime::currentDateTime().addSecs(-10);


    for(int i=0;i<lh.count();i++)
    {

        if(!i)
            m_minDate=lh[i].time;


        if(i && (!m_device->continousStreaming()))
            m_series->append(lh[i].time.toMSecsSinceEpoch(),lh[i-1].value);

        m_series->append(lh[i].time.toMSecsSinceEpoch(),lh[i].value);
        lastVal=lh[i].value;
    }


    if(m_device->maxValues()>3)
    {
         auto l=m_device->values();
         for(int i=0;i<l.count();i++)
         {

             if(i && (!m_device->continousStreaming()))
                 m_series->append(l[i].time.toMSecsSinceEpoch(),l[i-1].value);

             m_series->append(l[i].time.toMSecsSinceEpoch(),l[i].value);
            lastVal=l[i].value;
         }

    }

    if(m_series->count()<=1)
    {
       m_series->append(minDate.toMSecsSinceEpoch(),m_device->currentValue());
    }

    m_series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),lastVal);
    m_series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),m_device->currentValue());


    m_chart->addSeries(m_series);
    m_series->selectPoint(m_series->count()-1);
    m_yAxis->setRange(m_device->minRange()*1.1,m_device->maxRange()*1.1);
    m_chart->setTitle(m_device->name()+" : "+m_device->userValue());
    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);

    updateScale();
}

void DevicePlot::sliderChanged(int val)
{
    QDateTime now=QDateTime::currentDateTime();
    int max=m_minDate.secsTo(now);

    float r=101-val;
    r=r/100;

    float range=max*r*r;//quadratic
    setXRangeSecs(range);
}

Device *DevicePlot::device() const
{
    return m_device;
}

void DevicePlot::setXRangeSecs(int seconds)
{
    m_xRangeSecs = seconds;
    updateScale();
}

void DevicePlot::updateScale()
{
    QDateTime now=QDateTime::currentDateTime();

    if(m_xRangeSecs>0)
        m_xAxis->setRange(now.addSecs(-m_xRangeSecs),now);
    else
        m_xAxis->setRange(m_minDate,now);

    m_chart->update();
}
