#include "parameterplot.h"
#include <QScatterSeries>
#include <QDate>

ParameterPlot::ParameterPlot(QWidget* parent): QWidget(parent)
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
    m_chart->setTitle("Parameter Plot");

    // Set chart view layout
    m_view->setChart(m_chart);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_view);

    m_view->installEventFilter(this);
    setMinimumSize(500,500);

    m_xAxis->setRange(0,1000);
    m_yAxis->setRange(0,100);// change this !!
}

void ParameterPlot::refresh(Parameter *parameter)
{





    auto sel=m_series->selectedPoints();

    m_parameter = parameter;

    m_view->chart()->removeSeries(m_series);

    m_series->clear();

    // Add new data points
    for (const TimedValue &value : parameter->values()) {

        m_series->append(value.hourIndex, value.value);
    }





     m_yAxis->setRange(0,m_parameter->maxY());




    m_chart->setTitle(parameter->name());
    m_yAxis->setTitleText(parameter->units());

    if(!sel.isEmpty())
    {
        m_series->selectPoints(sel);
    }


    m_view->chart()->addSeries(m_series);

    qDebug()<<"freshch chart";

    initStyle();


}

void ParameterPlot::setXrange(int t)
{
    m_xAxis->setRange(0,t);
}

void ParameterPlot::initStyle()
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




void ParameterPlot::select(int a)
{
    m_series->deselectAllPoints();
    m_series->selectPoint(a);
}




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
