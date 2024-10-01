#include "slider.h"
#include "qpainter.h"
#include "qstyleoption.h"

Slider::Slider(QWidget *parent)
    : QSlider(parent)
{
    setMaximum(100);
    setMinimum(0);
    setOrientation(Qt::Horizontal);
    setFixedHeight(20);
}

void Slider::paintEvent(QPaintEvent *event)
{
    QStyleOptionSlider option;
    initStyleOption(&option);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the slider groove
    int w=3;
    int m=rect().height()/2-w;
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this).marginsRemoved(QMargins(m,m,m,m));


    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
    QPoint c=handleRect.center();

    QRect gr=grooveRect;
    gr.setLeft(handleRect.right());
    QRect gl=grooveRect;
    gl.setRight(handleRect.left());


    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().window());

    painter.drawRoundedRect(gr,w,w);

    painter.setBrush(palette().highlight());
    painter.drawRoundedRect(gl,w,w);

    painter.setPen(QPen(palette().highlight().color()));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(c,m,m);


}
