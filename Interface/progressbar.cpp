#include "progressbar.h"
#include "qpainter.h"
#include "qtimer.h"

ProgressBar::ProgressBar(QWidget *parent)
    : QWidget{parent},m_brush(),m_timer(nullptr)
{
    setFixedHeight(10);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
   // setMinimumWidth(150);
}

void ProgressBar::setCountDown(int ms, float val)
{
    m_maxCount=ms;
    m_count=ms*val;
    m_startTime=QTime::currentTime();

    if(!m_timer)
    {
        m_timer=new QTimer(this);
        connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    }
    m_timer->start(100);

}

void ProgressBar::paintEvent(QPaintEvent *)
{

      //QWidget::paintEvent(event);

      // Get progress bar geometry
      QRect rect = this->rect().marginsRemoved(QMargins(2,2,2,2));

      // Create a painter object
      QPainter painter(this);
      painter.setRenderHint(QPainter::Antialiasing);
      painter.setPen(QPen(palette().highlight().color()));
      painter.setBrush(Qt::NoBrush);

      painter.drawRoundedRect(rect,rect.height()/2,rect.height()/2);


      if(m_brush==QBrush())
        painter.setBrush(palette().highlight());
      else
        painter.setBrush(m_brush);


       QRect rb=rect.marginsRemoved(QMargins(2,2,2,2));

      // Implement your custom drawing logic here (e.g., different colors, gradients)
      int h=rb.height();

      int progressWidth = value() * (rb.width()-h) +h;
      rb.setWidth(progressWidth);

      painter.drawRoundedRect(rb,h/2,h/2);



}

void ProgressBar::timerSlot()
{
    m_count=m_startTime.msecsTo(QTime::currentTime());
    double v=m_count;
    v=v/m_maxCount;
    setValue(v);




    if(m_count>m_maxCount)
    {
        m_timer->stop();
        emit timeout();
    }
}

float ProgressBar::value() const
{
    return m_value;
}

void ProgressBar::setValue(double newValue)
{
    m_value = newValue;
    repaint();
}

void ProgressBar::setBrush(QBrush b)
{
    m_brush=b;
}
