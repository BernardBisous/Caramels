#include <QtCore>
#include <QtWidgets>
#include "progresswidget.h"
#include "toolbutton.h"



ProgressWidget::ProgressWidget(QWidget *parent) :
    TopWidget(parent),m_movie(nullptr),m_bar(nullptr),m_defaultWidth(-1)
{

    m_central->setLayout(new QVBoxLayout);
   // m_central->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);



   // QWidget*b=new QWidget;
  //  b->setLayout(new QVBoxLayout);

    m_central->layout()->addWidget(m_titleLabel=new QLabel);
    m_central->layout()->addWidget(m_videoLabel=new QLabel);
    m_central->layout()->addWidget(m_bar=new ProgressBar);
    m_central->layout()->addWidget(m_subTextLabel=new QLabel);
    m_central->layout()->addWidget(m_actionWidget=new QWidget);
    m_actionWidget->setLayout(new QHBoxLayout);
    m_actionWidget->layout()->setAlignment(Qt::AlignCenter);

    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_videoLabel->setAlignment(Qt::AlignCenter);


    connect(m_bar,SIGNAL(timeout()),this,SLOT(progressTimeoutSlot()));

    m_subTextLabel->setWordWrap(true);


    m_bar->setHidden(true);

    QFont f=m_titleLabel->font();
    f.setPixelSize(18);
    m_titleLabel->setFont(f);

    m_titleLabel->setWordWrap(true);
    m_bar->hide();
    m_subTextLabel->setWordWrap(true);
    m_subTextLabel->setAlignment(Qt::AlignCenter);

    setDefaultActionWidth(30);


}

void ProgressWidget::setEstimatedTime(int ms, float val)
{
    m_bar->setHidden(false);
    m_bar->setCountDown(ms,val);
}

void ProgressWidget::start(int timeout)
{
    TopWidget::start(timeout);

    if(m_movie)
    {
        m_videoLabel->setMovie(m_movie);
        m_movie->start();
    }
}

void ProgressWidget::stop()
{
    TopWidget::stop();
}

void ProgressWidget::setSubRichText()
{
    m_subTextLabel->setTextFormat(Qt::RichText);
}

void ProgressWidget::addActions(QStringList l)
{

    clearActions();
    for(int i=0;i<l.count();i++)
    {
        addAction(l[i]);
    }
}

ActionWidget *ProgressWidget::addAction(QString l, QString icon)
{
    auto a=new ToolButton(l,icon);
    a->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    if(m_defaultWidth>0)
        a->setIconSize(m_defaultWidth);

    a->setContentsMargins(0,0,0,0);
    addAction(a);
    return a;
}

void ProgressWidget::addAction(ActionWidget *b)
{
    connect(b,SIGNAL(clicked()),this,SLOT(actionSlot()));
    connect(b,SIGNAL(hovered(bool)),this,SLOT(actionHover(bool)));
    m_actionWidget->layout()->addWidget(b);
}

void ProgressWidget::clearActions()
{
    for(int i=0;i<m_actionWidget->layout()->count();i++)
    {
        auto a=m_actionWidget->layout()->itemAt(i)->widget();
        if(a )
        {
            auto b=dynamic_cast<ToolButton*>(a);
            if(b)
            {
                disconnect(b,SIGNAL(clicked()),this,SLOT(actionSlot()));
                disconnect(b,SIGNAL(hovered(bool)),this,SLOT(actionHover(bool)));
                delete b;
                i--;
            }
        }

    }
}

ActionWidget* ProgressWidget::addCancelAction(QString s, QString icon)
{
    clearActions();
    ActionWidget*c= addAction(s,icon);


    connect(c,SIGNAL(clicked()),this,SLOT(cancelSlot()));
    addAction(c);
    return c;

}

void ProgressWidget::setHoverVideo(QString s)
{
    m_hoverVideo=s;
}

void ProgressWidget::setTitleSize(int n)
{
    QFont f=m_titleLabel->font();
    f.setPixelSize(n);
    m_titleLabel->setFont(f);
}

void ProgressWidget::setText(QString title, QString subTitle)
{
    m_titleLabel->setText(title);
    m_subTextLabel->setText(subTitle);
    repaint();
}

void ProgressWidget::setMovie(QString path, bool s)
{
    if(s)
        m_videoPath=path;

    setMovie(new QMovie(path));
}

void ProgressWidget::setMovie(QMovie *m)
{

    if(m_movie)
        delete m_movie;

    if(!m)
        return;

    m_movie=m;
    m_movie->setScaledSize(QSize(150,150));
    m_videoLabel->setMovie(m_movie);
    m_movie->start();
}

void ProgressWidget::setPixmap(QPixmap p)
{
    m_videoLabel->setPixmap(p.scaled(m_videoLabel->size(),Qt::KeepAspectRatio));
    m_videoLabel->repaint();
}

void ProgressWidget::setMovieOnly()
{
    hideSpacers(true);
    m_titleLabel->setHidden(true);
    m_subTextLabel->setHidden(true);
}

void ProgressWidget::setActionHorizontal(bool s)
{
    if(s)
        dynamic_cast<QHBoxLayout*>(m_actionWidget->layout())->setDirection(QHBoxLayout::BottomToTop);
    else
        dynamic_cast<QHBoxLayout*>(m_actionWidget->layout())->setDirection(QHBoxLayout::LeftToRight);
}

void ProgressWidget::actionSlot()
{
    ToolButton*s=dynamic_cast<ToolButton*>(sender());
    if(s)
        emit actionTrigg(s->title(),s);


}

void ProgressWidget::actionHover(bool s)
{
    if(m_hoverVideo.isEmpty())
        return;

    if(s)
    {
        setMovie(m_hoverVideo,false);
    }
    else
        setMovie(m_videoPath,false);
}

void ProgressWidget::cancelSlot()
{
    hide();
    stop();
    emit canceled();
}

void ProgressWidget::progressTimeoutSlot()
{
    emit timeout();
}

int ProgressWidget::defaultWidth() const
{
    return m_defaultWidth;
}

void ProgressWidget::setDefaultActionWidth(int newDefaultWidth)
{
    m_defaultWidth = newDefaultWidth;
}





TopWidget::TopWidget(QWidget *parent):
    QWidget(parent),m_timer(nullptr),
   m_deleteAtEnd(false)
{
    hide();
    setAutoFillBackground(true);

  //  setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint );

    setLayout(new QHBoxLayout);
    layout()->addWidget(m_leftSpacer=new QWidget);
    QWidget*b=new QWidget;
    b->setLayout(new QVBoxLayout);
    b->layout()->setAlignment(Qt::AlignCenter);
    b->layout()->addWidget(m_topSpacer=new QWidget);
    b->layout()->addWidget(m_central=new QWidget);
    b->layout()->addWidget(m_bottomSpacer= new QWidget);
    layout()->addWidget(b);
    layout()->addWidget(m_rightSpacer=new QWidget);
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(0);

 //   m_central->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    m_leftSpacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_rightSpacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_topSpacer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    m_bottomSpacer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    if(!parent)
    {
        setWindowFlags(Qt::Window);
        setWindowModality(Qt::ApplicationModal);
        setAttribute(Qt::WA_DeleteOnClose);
    }
}

void TopWidget::start(int timeout)
{
    emit started();

    if(timeout>0)
    {
        if(!m_timer)
        {

            qDebug()<<"timerssserr";
            m_timer=new QTimer(this);
            connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
        }

        m_timer->start(timeout);
    }


    resetSize();
    if(parentWidget())
        parentWidget()->installEventFilter(this);

   show();
   raise();
}

void TopWidget::stop()
{
    if(m_timer)
        m_timer->stop();

    if(parentWidget())
        parentWidget()->removeEventFilter(this);
    hide();
    emit closed();

    if(m_deleteAtEnd)
        deleteLater();
}

void TopWidget::resetSize()
{
    if(!parentWidget())
        return;

    QRect r;
    r.setSize(parentWidget()->rect().size());
    setGeometry(r);

    if(m_autoresize && !m_pix.isNull())
    {
        QPalette p( palette() );
        p.setBrush(QPalette::Window,QBrush(m_pix.scaled(r.size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation)));
        setPalette(p);
    }
}

void TopWidget::setImage(QString url, bool autoResize)
{

    setImage(QPixmap(url),autoResize);

}

void TopWidget::setImage(QPixmap p, bool autoResize)
{
    m_pix=p.
            scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_autoresize=autoResize;

    QPalette pal( palette() );
    pal.setBrush(QPalette::Window,QBrush(QPixmap(m_pix)));
    m_central->setPalette(pal);
    m_central->repaint();
}

void TopWidget::timerSlot()
{
    emit timeout();
    stop();
}

bool TopWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==parentWidget() && event->type()==QEvent::Resize)
    {
        resetSize();
    }
    return QObject::eventFilter(obj, event);
   }

void TopWidget::setDeleteAtEnd(bool newDeleteAtEnd)
{
    m_deleteAtEnd = newDeleteAtEnd;
}

void TopWidget::hideSpacers(bool s)
{
    m_rightSpacer->setHidden(s);
    m_leftSpacer->setHidden(s);
    m_topSpacer->setHidden(s);
    m_bottomSpacer->setHidden(s);
}


AcknowledgeWidget::AcknowledgeWidget(QWidget *parent):ProgressWidget(parent)
{
    ActionWidget* c=addAction("Close");
    connect(c,SIGNAL(clicked()),this, SLOT(closeSlot()));
    setDeleteAtEnd(true);

}

void AcknowledgeWidget::show(QString title, QString sub, QString movie, int ms)
{
    setText(title, sub);
    setMovie(movie);
    start(ms);
}

void AcknowledgeWidget::closeSlot()
{
    stop();
}




TopGlobalWidget::TopGlobalWidget():
    TopWidget(QApplication::activeWindow())
{

}


ProgressBarWidget::ProgressBarWidget(QWidget *parent):TopWidget(parent) // Obsolete, TODO remove it
{
    m_central->setLayout(new QVBoxLayout);
    m_central->layout()->addWidget(m_title=new QLabel("Do you whant to update the database ?"));
    m_title->setWordWrap(true);
    m_central->layout()->addWidget(m_stateLabel=new QLabel);
    m_central->layout()->addWidget(m_progress=new QProgressBar);
    m_central->layout()->addWidget(m_startButton=new QPushButton("Download"));
    m_central->layout()->addWidget(m_cancel=new QPushButton("Cancel"));


    m_progress->setHidden(true);

    connect(m_startButton,SIGNAL(clicked()),this,SLOT(startSlot()));
     connect(m_cancel,SIGNAL(clicked()),this,SLOT(stopSlot()));

     m_progress->setMaximum(100);
     m_progress->setMinimum(0);



      m_progress->setAlignment(Qt::AlignCenter);
}

void ProgressBarWidget::startProgress(int toGet)
{
    m_toGet=toGet;
    m_got=0;
    m_progress->setHidden(false);
    m_startButton->setHidden(true);
}

void ProgressBarWidget::setState(QString s)
{
    if(s.isEmpty())
        m_stateLabel->setHidden(true);
    else
    {
        m_stateLabel->setHidden(false);
         m_stateLabel->setText(s);
    }
}

void ProgressBarWidget::inc(QString s)
{
    m_got++;
    int a=float(m_got*100/m_toGet);

    QString sd=QString::number(a)+"%";
    if(!s.isEmpty())
        sd+=" - "+s;

    if(!s.isEmpty())
    {
        setState(sd);

    }

    m_progress->setFormat(sd);
    m_progress->setValue(a);
    m_progress->update();
    m_progress->repaint();
   // qDebug()<<"PrintProgress"<<QString::number(a)+"% "+s;
}

void ProgressBarWidget::startSlot()
{

}

void ProgressBarWidget::stopSlot()
{
    emit timeout();
    stop();
}


int ProgressBarWidget::got() const
{
    return m_got;
}

int ProgressBarWidget::toGet() const
{
    return m_toGet;
}
