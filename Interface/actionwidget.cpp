#include "actionwidget.h"
#include "qapplication.h"
#include "qlabel.h"
#include "qtimer.h"
#include <QPainter>
#include <QtEvents>
#include <QLayout>

ActionWidget::ActionWidget(QWidget *parent)
    : QWidget{parent},m_hover(false),m_press(false),
      m_appear(false),m_radius(15),m_checked(false),
    m_hoverable(true),m_invertText(true), m_ignoreClick(false),m_mode(appearingBorder)
{

}

void ActionWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    if(!isEnabled()) //TODO do something when disabled
    {
        painter.restore();
        painter.end();
        QWidget::paintEvent(e);
        return;
    }




    if(m_appear && !m_hover)
    {
        painter.restore();
        painter.end();

        update();
        QWidget::paintEvent(e);
        return;
    }

     QBrush b1=backgroundBrush();




    painter.setBrush(b1);



    QPen p=borderPen();


    painter.setPen(p);


    float rx=m_radius;
    float ry=m_radius;
    painter.drawRoundedRect(e->rect().marginsRemoved(QMargins(p.width(),p.width(),p.width(),p.width())),rx,ry);

    painter.restore();
    painter.end();

    update();
    QWidget::paintEvent(e);
}

void ActionWidget::enterEvent(QEnterEvent *event)
{
    if(m_hoverable)
    {
        m_hover=true;
        emit hovered(true);


        if(m_invertText)
            setHighlightText(true);
    }

    QWidget::enterEvent(event);
}

void ActionWidget::leaveEvent(QEvent *event)
{
    if(m_hoverable)
    {

        emit hovered(false);

        m_hover=false;

        if(m_invertText)
            setHighlightText(false);
    }

    QWidget::leaveEvent(event);
}

void ActionWidget::mousePressEvent(QMouseEvent *e)
{

    QWidget::mousePressEvent(e);
    if(m_ignoreClick)
        return;

    emit clicked();

    m_press=true;
    e->accept();

}

void ActionWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);

    if(m_ignoreClick)
        return;

    e->accept();
    m_press=false;

}

QBrush ActionWidget::backgroundBrush()
{
    QBrush b1;
    if(m_press && !m_ignoreClick)
        b1= palette().highlight();

    else if(!m_hover)
        b1= palette().base();

    else
        b1= palette().window();

    return b1;
}

QPen ActionWidget::borderPen()
{
    QBrush b=backgroundBrush();
    QPen p;
    p.setColor(palette().highlight().color());

    if(m_checked)
        p.setColor(palette().highlight().color());

    else if(m_mode==noBorder || (m_appear && !m_hover))
        p.setColor(b.color());

    else if(m_mode==highlighted)
        p.setColor(palette().text().color());

    else if(m_mode==customPen)
        p=m_borderPen;

    else if(m_mode==appearingBorder)
    {
        if(m_hover)
            p.setColor(palette().highlight().color());
        else
            p.setColor(b.color());
    }


    if(m_press)
        p.setWidth(2); //border hoverd

    else
        p.setWidth(1);  //border not used

    return p;

}

void ActionWidget::animateClick()
{
    m_press=true;
    repaint();
    QTimer::singleShot(200,this,[this](){m_press=false;repaint();});
}


void ActionWidget::setAppear(bool newAppear)
{
    m_appear = newAppear;
}

void ActionWidget::setRadius(int newRadius)
{
    m_radius = newRadius;
}

void ActionWidget::setHover(bool newHover)
{
    m_hover = newHover;
}

void ActionWidget::setCheckable(bool s)
{
    m_checkable=s;
}

void ActionWidget::setChecked(bool s)
{
    m_checked=s;
}

void ActionWidget::setBorderPen(QPen p)
{
    setMode(customPen);
    m_borderPen=p;
}

bool ActionWidget::hoverable() const
{
    return m_hoverable;
}

void ActionWidget::setHoverable(bool newHoverable)
{
    m_hoverable = newHoverable;
}

ActionWidget::DisplayMode ActionWidget::mode() const
{
    return m_mode;
}

void ActionWidget::setMode(DisplayMode newMode)
{
    m_mode = newMode;
/*
    if(newMode==highlighted)
        setContentsMargins(5,5,5,5);
    else setContentsMargins(0,0,0,0);
    */
}

bool ActionWidget::invertText() const
{
    return m_invertText;
}

void ActionWidget::setInvertingText(bool newInvertText)
{
    m_invertText = newInvertText;
}

void ActionWidget::setHighlightText(bool s)
{
    for(int i=0;i<layout()->count();i++)
    {
        auto w=layout()->itemAt(i)->widget();
        auto l=dynamic_cast<QLabel*>(w);
        if(l)
        {
            if(s)
                l->setForegroundRole(QPalette::HighlightedText);
            else
                l->setForegroundRole(QPalette::Text);
        }
    }
    /*

    QPalette p=QApplication::palette();

    if(s)
        p.setColor(QPalette::Text,QColor(p.color(QPalette::HighlightedText)));

    setPalette(p);
    update();
    repaint();
    */
}

bool ActionWidget::ignoreClick() const
{
    return m_ignoreClick;
}

void ActionWidget::setIgnoreClick(bool newIgnoreClick)
{
    m_ignoreClick = newIgnoreClick;
}

bool ActionWidget::checked() const
{
    return m_checked;
}

