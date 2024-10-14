#include "unitplot.h"
#include <qsplineseries.h>

UnitPlot::UnitPlot(QWidget *parent): QWidget(parent),m_client(nullptr),m_xRangeMs(0)
  ,m_highlightedDevice(nullptr),m_highlightedParameter(nullptr)
{

    qDebug()<<"timererreeeeww";
    m_timer=new QTimer(this);
    m_timer->setInterval(300);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateSensors()));

    m_view = new QChartView(this);
    m_view->setRenderHint(QPainter::Antialiasing);


    // Create axes
    m_xAxis = new QDateTimeAxis;
    m_xAxis->setFormat("hh:mm:ss");
    m_xAxis->setTitleText("Time");
    m_yAxis = new QValueAxis;
    m_xAxis->setTitleVisible(true);

    m_yAxis->setVisible(false);
    m_yAxis->setRange(0,100);

    // Set axis properties




    // Create the chart and add the series and axes
    m_chart = new QChart();
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);


    m_view->setChart(m_chart);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_view);
    layout()->setContentsMargins(0,0,0,0);

    layout()->addWidget(m_rangeSlider=new Slider);
    m_rangeSlider->setOrientation(Qt::Horizontal);
    connect(m_rangeSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderChanged(int)));

    setMinimumSize(200,200);

    m_rangeSlider->hide();
    initStyle();

}

void UnitPlot::removeMargins()
{
    layout()->setContentsMargins(0,0,0,0);

    m_view->setContentsMargins(0,0,0,0);
}

void UnitPlot::enable(bool s)
{
    if(s)
        m_timer->start(s);

    else
        m_timer->stop();
}

void UnitPlot::handle(HardwareUnit *u, QList<Device *> devices, QList<Parameter *> parameters)
{
    initStyle();
    m_client=u;

    if(!u)
        return;

    m_startDate=u->startTime();

    if(!parameters.isEmpty())
        m_chart->setTitle(parameters.first()->name());
    else
        m_chart->setTitle(u->name());

    setDevices(devices);
    setParameters(parameters);

    updatePlot();

    if(!m_devicesSeries.isEmpty())
        m_timer->start();
}



void UnitPlot::updateParameters()
{
    m_startDate=m_client->startTime();

    if(m_highlightedParameter)
    {

        int index=m_parameters.indexOf(m_highlightedParameter);

        if(index>=0)
            updateParameter(m_parametersSeries[index],m_highlightedParameter,m_startDate);
    }


    else
    {
        for(int j=0;j<m_parameters.count();j++)
        {
            updateParameter(m_parametersSeries[j],m_parameters[j],m_startDate);
        }
    }


    updateScale();
}

void UnitPlot::sliderChanged(int i)
{
    if(i==0)
    {
        m_xRangeMs=0;
        return;

    }

    int totalMs=m_startDate.msecsTo(endConfig());
    float s=totalMs;
    s=s/(Parameter::timeMultiplicator()*i*i/300);
    m_xRangeMs=s;
    updateScale();
}

QList<Parameter *> UnitPlot::parameters() const
{
    return m_parameters;
}

void UnitPlot::setParameters(const QList<Parameter *> &newParameters)
{
    m_parameters = newParameters;
    initSeriesParemeters();
}

Device *UnitPlot::attachedDevice(Parameter *p)
{
    for(int i=0;i<m_devices.count();i++)
        if(m_devices[i]->parameter()==p)
            return m_devices[i];

    return nullptr;
}

QList<Device *> UnitPlot::devices() const
{
    return m_devices;
}

void UnitPlot::setDevices(const QList<Device *> &newDevices)
{
    m_devices = newDevices;

    initSeriesDevices();
}


void UnitPlot::updateScale()
{

    if(!m_highlightedDevice && !m_highlightedParameter)
    {
        m_yAxis->setMax(110);
        m_yAxis->setMin(-10);
    }


    QDateTime min;
    QDateTime max;

    if(m_xRangeMs>0)
    {
        max=QDateTime::currentDateTime();
        min=max.addMSecs(-m_xRangeMs);

        /*
        if(min<m_startDate)
        {
            int mss=min.msecsTo(m_startDate);
            min=m_startDate;
            max=max.addMSecs(mss);
        }
        */
    }
    else
    {
        max=endConfig();
        min=m_startDate;
    }

    m_xAxis->setRange(min,max);

}

void UnitPlot::updatePlot()
{
    updateParameters();
    updateSensors();

}

void UnitPlot::updateSensors()
{





    if(m_highlightedDevice)
    {
        int index=m_devices.indexOf(m_highlightedDevice);

        if(index>=0)
            updateSensor(m_devicesSeries[index],m_highlightedDevice);
    }

    else
    {
        for(int i=0;i<m_devices.count();i++)
        {
            updateSensor(m_devicesSeries[i],m_devices[i]);
        }
    }



    updateScale();
}




HardwareUnit *UnitPlot::client() const
{
    return m_client;
}

void UnitPlot::setStartDate(const QDateTime &newStartDate)
{
    m_startDate = newStartDate;

}

QLineSeries *UnitPlot::paramSerie(Parameter *p)
{

    int index=m_parameters.indexOf(p);
    if(p&&index>=0)
    {
        return m_parametersSeries[index];
    }
    return nullptr;
}

QLineSeries *UnitPlot::deviceSerie(Device *d)
{
    int index=m_devices.indexOf(d);
    if(d&&  index>=0 )
    {
        return m_devicesSeries[index];
    }
    return nullptr;
}

void UnitPlot::initSeriesParemeters()
{
    for(int i=0;i<m_parametersSeries.count();i++)
        delete m_parametersSeries[i];
    m_parametersSeries.clear();

    if(m_parameters.isEmpty()&&m_client)
        m_parameters=m_client->parameters();

    for(int i=0;i<m_parameters.count();i++)
    {
        QLineSeries* s=new QLineSeries;
        m_parametersSeries.append(s);
        s->setPointsVisible();
        m_chart->addSeries(s);
        s->attachAxis(m_xAxis);
        s->attachAxis(m_yAxis);
    }
}

void UnitPlot::initSeriesDevices()
{
    for(int i=0;i<m_devicesSeries.count();i++)
        delete m_devicesSeries[i];
    m_devicesSeries.clear();

    if(m_devices.isEmpty()&&m_client)
        m_devices=m_client->devices();
    QPen pen(palette().highlight().color());
    pen.setStyle(Qt::DotLine);

    for(int i=0;i<m_devices.count();i++)
    {
        Device* d=m_devices[i];
        QLineSeries * s=new QLineSeries;
        m_devicesSeries.append(s);

        pen.setColor(palette().highlight().color());

        if(d->parameter())
        {
            int ind=m_parameters.indexOf(d->parameter());
            if(ind>=0)
            {
                 QColor c=m_parametersSeries[ind]->color();
                 pen.setColor(c);
            }
        }
        s->setPen(pen);
        m_chart->addSeries(s);
        s->attachAxis(m_xAxis);
        s->attachAxis(m_yAxis);
    }
}

void UnitPlot::hideAll(bool s)
{
    auto l=m_chart->series();
    for(int i=0;i<l.count();i++)
    {
        if(s)
            l[i]->show();
        else
            l[i]->hide();
    }
}


void UnitPlot::highlight(Parameter *p)
{


    m_highlightedParameter=p;

    m_highlightedDevice=attachedDevice(p);

    updateHighlight();

}

void UnitPlot::highlight(Device *d)
{


    m_highlightedDevice=d;
    if(d)
    {
        m_highlightedParameter=d->parameter();
    }

    updateHighlight();
}

void UnitPlot::highlight(QList<QLineSeries *> l)
{
    auto ls=m_chart->series();
    for(int i=0;i<ls.count();i++)
    {
        if(l.contains(ls[i]))
            ls[i]->show();
        else
            ls[i]->hide();
    }


    updatePlot();
}

void UnitPlot::updateHighlight()
{



    QList<QLineSeries *> sl;
    auto p=paramSerie(m_highlightedParameter);
    auto d=deviceSerie(m_highlightedDevice);

    if(p)
        sl<<p;
    if(d)
        sl<<d;

    highlight(sl);

}





QDateTime UnitPlot::endConfig()
{
    if(!m_client)
        return QDateTime::currentDateTime();
    return m_client->endConfig();
}

void UnitPlot::updateSensor(QLineSeries *series, Device *s)
{
    if(m_chart->series().contains(series))
        m_chart->removeSeries(series);

    series->clear();

    QList<RealTimeValue> in=s->values();

    auto l=s->values();
    auto lh=s->historic();

    for(int i=0;i<lh.count();i++)
    {
        if(!s->continousStreaming() && i)
            series->append(lh[i].time.toMSecsSinceEpoch(),s->mapToPurcent(lh[i-1].value));

        series->append(lh[i].time.toMSecsSinceEpoch(),s->mapToPurcent(lh[i].value));
    }


    for(int i=0;i<l.count();i++)
    {
        if(!s->continousStreaming() && i)
            series->append(l[i].time.toMSecsSinceEpoch(),s->mapToPurcent(l[i-1].value));

        series->append(l[i].time.toMSecsSinceEpoch(),s->mapToPurcent(l[i].value));
    }

    series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),s->currentValue());
    m_chart->addSeries(series);
    series->attachAxis(m_xAxis);
    series->attachAxis(m_yAxis);
}

void UnitPlot::updateParameter(QLineSeries *l, Parameter *p,QDateTime start)
{
    if(m_chart->series().contains(l))
        m_chart->removeSeries(l);

    l->clear();
    int n=p->count();

    for(int i=0 ; i<n ;i++)
    {
        auto v=p->at(i);
        QDateTime te=start.addSecs(v.hourIndex*Parameter::timeMultiplicator());

        l->append(te.toMSecsSinceEpoch(),v.value);

    }


    m_chart->addSeries(l);
    l->attachAxis(m_xAxis);
    l->attachAxis(m_yAxis);
}

void UnitPlot::initStyle()
{

    QColor gridColor(palette().alternateBase().color());
    QColor textColor(palette().text().color());

    m_chart->setBackgroundBrush(palette().base());
   // m_series->setColor(palette().highlight().color());
    //m_chart->setPlotAreaBackgroundPen(QPen(Qt::yellow));

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
    m_view->update();
    m_view->repaint();
    repaint();
    update();


}

float UnitPlot::maxY()
{
    return 100;
}



