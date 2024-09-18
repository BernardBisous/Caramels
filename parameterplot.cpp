#include "parameterplot.h"
#include "qpaintengine.h"
#include <QScatterSeries>
#include <QDate>
#include <QGraphicsLayout>
ParameterPlot::ParameterPlot(QWidget* parent): QWidget(parent),
    m_parameter(nullptr),m_startDate()
{
    m_view = new QChartView(this);
    m_view->setRenderHint(QPainter::Antialiasing);

    // Create a line series to hold the data points
    m_series = new QLineSeries();
    m_series->setPointsVisible();

    // Create axes
    m_xAxis = new QValueAxis();
    m_yAxis = new QValueAxis();

    // Set axis properties
    m_xAxis->setTitleText("Hours");
    m_yAxis->setTitleText("Value");

    // Create the chart and add the series and axes
    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);

    // Set chart title


    // Set chart view layout
    m_view->setChart(m_chart);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_view);
    m_chart->layout()->setContentsMargins(0,0,0,0);
    layout()->setContentsMargins(0,0,0,0);
    m_view->setContentsMargins(0,0,0,0);
    m_view->installEventFilter(this);


    m_series->setSelectedColor(Qt::white);

    m_yAxis->setTitleVisible(false);
    m_xAxis->setVisible(false);


    setMinimumSize(1,10);
    initStyle();
}

void ParameterPlot::refresh()
{
    if(!m_parameter)
        return;


    auto sel=m_series->selectedPoints();

    if(m_chart->series().contains(m_series))
        m_view->chart()->removeSeries(m_series);



    m_series->clear();


    for (const TimedValue &value : m_parameter->values()) {

        m_series->append(value.hourIndex, value.value);
    }


    float max,min;
    m_parameter->rangeY(&max,&min);

    float err=qAbs(max-min);
    err=err*0.1;


    m_yAxis->setTitleText(m_parameter->units());

    if(!sel.isEmpty())
    {
        m_series->selectPoints(sel);
    }


    m_view->chart()->addSeries(m_series);
    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);

    m_xAxis->setRange(-5,m_parameter->maxX()+5);
    m_yAxis->setRange(min-err,max+err);

   refreshTitle();
}

QDateTime ParameterPlot::startDate() const
{
    return m_startDate;
}

void ParameterPlot::setStartDate(QDateTime newStartDate)
{
    m_startDate = newStartDate;
    selectDefault();
}

int ParameterPlot::currentTimeIndex()
{
    if(!m_startDate.isValid() || !m_parameter)
        return -1;

    int elapsedH=m_startDate.secsTo(QDateTime::currentDateTime())/Parameter::timeMultiplicator();

    return m_parameter->closerIndex(elapsedH);
}

int ParameterPlot::closerIndexPoint(int hourIndex)
{
    return m_parameter->closerIndex(hourIndex);
}

void ParameterPlot::refreshTitle()
{
    if(!m_parameter)
        return;

    auto l=m_series->selectedPoints();
    if(!l.isEmpty())
    {
        QString value=m_parameter->userValueAt(l.first());
        m_chart->setTitle(m_parameter->name() +": "+value);
    }
}

Parameter *ParameterPlot::parameter() const
{
    return m_parameter;
}

void ParameterPlot::setXrange(int t)
{
   // m_xAxis->setRange(0,t);
}

void ParameterPlot::initStyle()
{
    QColor gridColor(palette().alternateBase().color());
    QColor textColor(palette().text().color());

    m_chart->setBackgroundBrush(palette().base());
    m_series->setColor(palette().highlight().color());
    m_chart->setPlotAreaBackgroundPen(QPen(Qt::yellow));
    m_chart->setPlotAreaBackgroundVisible(true);

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

    m_chart->setPlotAreaBackgroundPen(gridColor);

    m_chart->legend()->hide();
    m_chart->update();
}




void ParameterPlot::select(int a)
{
    m_series->deselectAllPoints();



    if(a<0)
        return;


    m_series->selectPoint(a);

    refreshTitle();
}

void ParameterPlot::selectDefault()
{
    if(!m_parameter)
        return;

    select(currentTimeIndex());
}

QLineSeries *ParameterPlot::series() const
{
    return m_series;
}

void ParameterPlot::setParameter(Parameter *p)
{
    m_parameter=p;
    refresh();
    selectDefault();
}





/*
bool ParameterPlot::eventFilter(QObject *obj, QEvent *event)
{

    if (obj==m_view && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
    {
            // Handle mouse click here, e.g., emit a signal


            QPoint pe=mouseEvent->pos();
            pe.setY(-pe.y());

            QPointF  chartPoint = m_view->mapToScene(pe);
          //  chartPoint.setY()
            qDebug()<<"clickedAt"<<pe<<chartPoint;



            auto const widgetPos = mouseEvent->position();
            auto const scenePos = m_view->mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
            auto const chartItemPos = m_view->chart()->mapFromScene(scenePos);
            auto const valueGivenSeries =m_view-> chart()->mapToValue(chartItemPos);
            qDebug() << "widgetPos:" << widgetPos;
            qDebug() << "scenePos:" << scenePos;
            qDebug() << "chartItemPos:" << chartItemPos;
            qDebug() << "valSeries:" << valueGivenSeries;



            emit clicked(chartPoint);

            return true; // Event handled
        }
    }

    return false; // Event not handled
}
*/
