#include "toolbutton.h"
#include <QMovie>
#include <QLabel>
#include <QBoxLayout>
#include <QIcon>

ToolButton::ToolButton(QString title, QString icon, QWidget *parent) :
    ActionWidget (parent),m_movie(nullptr)
{
    setCheckable(true);

    setLayout(new QHBoxLayout);
    layout()->addWidget(m_icon=new QLabel);
    layout()->addWidget(m_title=new QLabel);
    layout()->setSpacing(0);

    m_icon->setAlignment(Qt::AlignCenter);
    m_title->setAlignment(Qt::AlignCenter);
    m_title->setAlignment(Qt::AlignVCenter);

    setLayoutMode(horizontal);
    setText(title);
    setIconSize(50);
    setIcon(icon);
}

void ToolButton::setMovie(QString s)
{
    auto m=new QMovie(s);

    if(s.isEmpty() || !m->isValid())
    {
        delete m;
        m_movie=nullptr;
        m_icon->setHidden(true);
        return;
    }

    setMovie(m);

}

void ToolButton::setMovie(QMovie *m)
{
    if(m_movie)
        delete m_movie;

    m_movie=m;
    m_icon->setHidden(false);
    m_icon->setMovie(m_movie);
    m_movie->setScaledSize((m_icon->size()));
    m_movie->start();
}

void ToolButton::setIcon(QString s)
{
    if(s.isEmpty())
    {

        setLayoutMode(textOnly);
        return;
    }
    else
        m_icon->setHidden(false);

    setIcon(QIcon(s));
}

void ToolButton::setIcon(QIcon i)
{
    if(i.isNull())
    {
        m_icon->setHidden(true);
        return;
    }

    m_icon->setPixmap(i.pixmap(m_icon->size()));
    m_icon->setHidden(m_layoutMode==textOnly);
}



void ToolButton::setText(QString s)
{
    m_title->setText(s);

    if(s.isEmpty())
        setLayoutMode(iconOnly);

    m_title->setHidden(s.isEmpty() || m_layoutMode==iconOnly);
}

void ToolButton::setFontSize(int i)
{
    QFont f=m_title->font();
    f.setPointSize(i);
    m_title->setFont(f);
}

void ToolButton::setIconSize(QSize s)
{
    m_icon->setFixedSize(s);

    QPixmap p=m_icon->pixmap();
    if(!p.isNull())
    {
        m_icon->setPixmap(p.scaled(m_icon->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}

void ToolButton::setIconSize(int s)
{
    setIconSize(QSize(s,s));
}

void ToolButton::setLayoutMode(LayoutMode m)
{
    m_layoutMode=m;
    //TODO Implement other mode
    auto l=dynamic_cast<QBoxLayout*>(layout());

    switch(m)
    {
    case vertical: l->setDirection(QBoxLayout::TopToBottom);break;
    case horizontal: l->setDirection(QBoxLayout::LeftToRight);break;
    default: break;
    }

    if(m==horizontal)
    {
        m_title->setAlignment(Qt::AlignLeft);
        m_title->setAlignment(Qt::AlignVCenter);
    }
    else
        m_title->setAlignment(Qt::AlignCenter);

    m_title->setHidden(m==iconOnly);
    m_icon->setHidden(m==textOnly);

    /*// it this thas this has no effect but bad ones
    if(m==iconOnly)
        setIconSize(50);
    else
        setIconSize(40);
        */

    if(m==iconOnly)
        setToolTip(m_title->text());
}

QString ToolButton::title()
{
    return m_title->text();
}

void ToolButton::setRound(int d,int m)
{
    setLayoutMode(iconOnly);


    setFixedSize(QSize(d,d));

    int n=d-2*m;
    setIconSize(QSize(n,n));

    setRadius(d/2);
    setMode(ActionWidget::noBorder);
    layout()->setContentsMargins(m,m,m,m);

}

QMovie *ToolButton::movie() const
{
    return m_movie;
}







ToolButtonText::ToolButtonText(QString title, QString icon, QString subText, QWidget *parent):
    ToolButton(title,icon,parent)
{
    while(layout()->takeAt(0));

    dynamic_cast<QBoxLayout*>(layout())->setDirection(QBoxLayout::LeftToRight);

    QWidget* b=new QWidget;
    b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    b->setLayout(new QVBoxLayout);
    b->layout()->addWidget(m_title);
    b->layout()->addWidget(m_subText=new QLabel);
    b->setContentsMargins(0,0,0,0);

    layout()->addWidget(m_icon);
    layout()->addWidget(b);

    QFont f=m_title->font();
    f.setPointSize(16);
    m_title->setFont(f);

    m_subText->setWordWrap(true);
    setSubText(subText);


}


void ToolButtonText::setSubText(QString s)
{
    m_subText->setText(s);
    m_subText->setHidden(s.isEmpty());
}

QString ToolButtonText::subText()
{
    return m_subText->text();
}
