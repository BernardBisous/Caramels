#include "webcamoverview.h"
#include "qevent.h"
#include "qtimer.h"


CameraOverview::CameraOverview(QWidget *parent):
    QLabel(parent),m_frameDelay(100),m_client(nullptr),m_pix()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &CameraOverview::updatePixmap);
    setAlignment(Qt::AlignCenter);
}

void CameraOverview::load()
{
    /*
    m_pixmaps.clear();
    QStringList ls=m_client->picsFiles();
    for(int i=0;i<ls.count();i++)
    {
        QPixmap ps=QPixmap(ls[i]);
        if(!ps.isNull())
        {
           m_pixmaps.append(ps.scaled(size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
        }
    }
    */
}

void CameraOverview::loadFirst()
{

    QStringList ls=m_client->picsFiles();
    if(!ls.isEmpty())
    {

        QPixmap ps=QPixmap(ls.last());

        if(!ps.isNull())
        {
           m_pix=ps;
           printPixmap();
        }
    }
}

void CameraOverview::reset()
{
    m_counter=0;
    m_timer->stop();
    loadFirst();
}

void CameraOverview::start()
{
    m_pixNames=m_client->picsFiles();
    m_counter=0;
    load();
    m_timer->start(m_frameDelay);

}

void CameraOverview::handle(Webcam *w)
{
    m_client=w;
    connect(w,SIGNAL(saved(QString)),this,SLOT(pixSlot(QString)));
    reset();
}

void CameraOverview::updatePixmap()
{

    if(m_counter>=0)
    {
        m_pix=QPixmap(m_pixNames[m_counter]);
        m_counter = (m_counter + 1) % m_pixNames.size();
    }


    printPixmap();

}

void CameraOverview::pixSlot(QString s)
{

    QPixmap ps=QPixmap(s);

    if(ps.isNull())
        return;

    if(m_timer->isActive())
        m_pixNames.append(s);
    else
        m_pix=ps;

    printPixmap();

}

int CameraOverview::frameDelay() const
{
    return m_frameDelay;
}

void CameraOverview::setFrameDelay(int newFrameDelay)
{
    m_frameDelay = newFrameDelay;
}

void CameraOverview::resizeEvent(QResizeEvent *e)
{

   QLabel::resizeEvent(e);
   updateGeometry();
   printPixmap();
}

void CameraOverview::printPixmap()
{
    if(m_pix.isNull())
    {
        clear();
        return;
    }

    setPixmap(m_pix.scaled(size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}
