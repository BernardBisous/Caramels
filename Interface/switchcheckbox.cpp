#include "switchcheckbox.h"
#include "qevent.h"
#include "qpainter.h"
#include "qstyleoption.h"


SwitchCheckBox::SwitchCheckBox(QString s, QWidget *parent)
    : QCheckBox{s,parent}
{
    //setWidth(50);
    //setMinimumHeight(50/3);
}

void SwitchCheckBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
QPainter painter(this);


    painter.setRenderHint(QPainter::Antialiasing);

    //painter.setPen(Qt::NoPen);
    painter.setPen(palette().highlight().color());


    int h=height()*0.7;

    if(h>50)
        h=50;

    int w=2*h;


    QRect r(QPoint(0,0),QPoint(w,h));
    r=r.translated((height()-h)/2,(height()-h)/2);

    QPoint p(r.topLeft()+QPoint(h/2,h/2+8));
    if(isChecked())
    {
        painter.setBrush(palette().brush(QPalette::Highlight));
        p=r.center()+QPoint(h-5,0);
    }
    else
    {
        p=r.center()-QPoint(h-5,0);
        painter.setBrush(palette().brush(QPalette::Base));
    }

    painter.drawRoundedRect(r,h/2,h/2);
    painter.setBrush(palette().brush(QPalette::Text));
    painter.drawEllipse(p,h/2+1,h/2+1);



    // Draw the checkbox label
    painter.setPen(palette().color(QPalette::Text));
    painter.drawText(rect().adjusted(w+h+5, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, text());


}

void SwitchCheckBox::mousePressEvent(QMouseEvent *event)
{
    toggle();
    event->accept();
    /*
    // Toggle the state on mouse press
    if (event->button() == Qt::LeftButton) {
        toggle();
        event->accept();
    } else {
       QCheckBox::mousePressEvent(event);
    }
    */
}

