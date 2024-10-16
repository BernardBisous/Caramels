#include "scrollarea.h"
#include "Interface/toolbutton.h"
#include "qboxlayout.h"
#include "qevent.h"
#include "qpainter.h"
#include "qstyle.h"
#include "qstyleoption.h"



ScrollArea::ScrollArea(bool h,QWidget *parent)
    : QScrollArea{parent}
{

    QWidget* w=new QWidget(this);
    w->setLayout(new QVBoxLayout);

    setWidget(w);

    setFrameStyle(QFrame::NoFrame);
    setWidgetResizable(true);
    setHorizontalScrollBar(new ScrollBarHorizontal);
    setVerticalScrollBar(new ScrollBarVerical);

    setHorizontal(h);
}

void ScrollArea::enterEvent(QEnterEvent *event)
{
    auto a=dynamic_cast<ScrollBarVerical*> (verticalScrollBar());
    a->setHover(true);
    a->repaint();

    QScrollArea::enterEvent(event);
}

void ScrollArea::leaveEvent(QEvent *event)
{
    auto a=dynamic_cast<ScrollBarVerical*> (verticalScrollBar());
    a->setHover(false);
    a->repaint();

    QScrollArea::leaveEvent(event);
}

void ScrollArea::setHorizontal(bool b)
{
    m_horizontal=b;

    if(!widget())
        return;

    auto l=dynamic_cast<QBoxLayout*>(widget()->layout());

    if(!l)
        return;

    if(b)
    {
        l->setDirection(QBoxLayout::LeftToRight);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        l->setDirection(QBoxLayout::TopToBottom);
    }
}

void ScrollArea::clear()
{

    if(!widget())
    {
        return;
    }
    while(widget()->layout()->count())
    {
        auto i= widget()->layout()->takeAt(0);
        if(i)
        {


            if(i->widget())
            {
                ActionWidget*a=dynamic_cast<ActionWidget*>(i->widget());
                if(a)
                  disconnect(a, SIGNAL(clicked()),this,SLOT(trigSlot()));

                delete i->widget();
            }
            delete i;
        }
    }
}

void ScrollArea::removeMargins()
{
if(widget() && widget()->layout())
    widget()->layout()->setContentsMargins(0,0,0,0);
}

void ScrollArea::fillList(QStringList s)
{
    clear();
    for (int i=0;i<s.count();i++)
        addActionText(s[i]);

    addSpacer();
}

void ScrollArea::setCurrent(int index)
{
    auto ls=actionWidgets();
    for(int i=0;i<ls.count();i++)
    {
        ls[i]->setChecked(i==index);
    }
}

void ScrollArea::addWidget(QWidget*w)
{
    if(!widget())
    {
        return;
    }

    widget()->layout()->addWidget(w);
}

void ScrollArea::addActionText(QString s)
{

  ToolButton* a=new ToolButton(s);
  a->setMode(ActionWidget::appearingBorder);
  addActionWidget(a);
}

void ScrollArea::addActionWidget(ActionWidget *a)
{
    connect(a, SIGNAL(clicked()),this,SLOT(trigSlot()));
    addWidget(a);
}

void ScrollArea::addActionText(QString text, QString icon)
{
    addActionWidget(new ToolButton(text,icon));
}
void ScrollArea::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(m_horizontal)
        widget()->setFixedHeight(height());
    else
        widget()->setFixedWidth(width());

    widget()->updateGeometry();
    setWidget(widget());
}

QWidget *ScrollArea::at(int i)
{
    if(!widget() || !widget()->layout() || widget()->layout()->count()<=i)
        return nullptr;
    return widget()->layout()->itemAt(i)->widget();

}

int ScrollArea::indexOf(QWidget *w)
{
    return widgets().indexOf(w);
}

QList<ActionWidget *> ScrollArea::actionWidgets()
{
    QList<ActionWidget *> out;
    for(int i=0;i<widget()->layout()->count();i++)
    {
        auto it=widget()->layout()->itemAt(i);
                  if(it && it->widget())
                  {
                      ActionWidget* a=dynamic_cast<ActionWidget*>(it->widget());
                      if(a)
                        out<<a;
                  }

    }
    return out;
}

QList<QWidget *> ScrollArea::widgets()
{
    QList<QWidget *> out;
    for(int i=0;i<widget()->layout()->count();i++)
    {
        auto it=widget()->layout()->itemAt(i);
                  if(it && it->widget())
            out<<it->widget();
    }
    return out;
}

void ScrollArea::scrollDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

ActionWidget *ScrollArea::currentAction()
{
    auto ls=actionWidgets();
    for(int i=0;i<ls.count();i++)
    {
        if(ls[i]->checked())
            return ls[i];
    }
    return nullptr;

}

void ScrollArea::trigSlot()
{
    QWidget* w=dynamic_cast<ActionWidget*>(sender());
    if(w)
        emit trigger(indexOf(w),w);

}
void ScrollArea::addSpacer()
{
    QWidget* sp=new QWidget;
    sp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    addWidget(sp);
}


ScrollBarHorizontal::ScrollBarHorizontal(QWidget *parent):QScrollBar{parent}
{

}

void ScrollBarHorizontal::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    auto style_option = QStyleOptionSlider();
    initStyleOption(&style_option);
    auto sliderRect = style()->subControlRect(QStyle::CC_ScrollBar,
                                              &style_option, QStyle::SC_ScrollBarSlider);

    QPen pen(palette().highlight().color());
    painter.setPen(pen);
    painter.setBrush(palette().highlight());

    painter.drawRoundedRect(rect().marginsRemoved(QMargins(0,5,0,5)), 1,1);
    painter.drawRoundedRect(sliderRect.marginsRemoved(QMargins(0,1,0,1)), 5,5);

    int h=rect().height()/2;
    QPoint pl=rect().topLeft()+QPoint(h,h);
    QPoint pr=rect().topRight()+QPoint(-h,h);
    painter.drawEllipse(pl,h,h);
    painter.drawEllipse(pr,h,h);
    event->accept();
}


ScrollBarVerical::ScrollBarVerical(QWidget *parent):QScrollBar{parent},m_hover(false)
{

}



void ScrollBarVerical::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    auto style_option = QStyleOptionSlider();
    initStyleOption(&style_option);
    auto sliderRect = style()->subControlRect(QStyle::CC_ScrollBar,
                                              &style_option, QStyle::SC_ScrollBarSlider);


    QPen pen(palette().highlight().color());
    painter.setPen(pen);
    painter.setBrush(palette().highlight());



    painter.drawRoundedRect(rect().marginsRemoved(QMargins(7,0,7,0)), 1,1);
    if(m_hover)
    {
        painter.drawRoundedRect(sliderRect.marginsRemoved(QMargins(1,0,1,0)), 5,5);
        int h=rect().width()/2;
        int w=3;
        QPoint pl=rect().topLeft()+QPoint(h,w);
        QPoint pr=rect().bottomLeft()+QPoint(h,-w);

        h--;
        painter.drawEllipse(pl,w,w);
        painter.drawEllipse(pr,w,w);
    }






    event->accept();
}

void ScrollBarVerical::setHover(bool newHover)
{
    m_hover = newHover;
}
