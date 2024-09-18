#include "webcamoverview.h"
#include "qevent.h"
#include "qtimer.h"


CameraOverview::CameraOverview(QWidget *parent):
    QLabel(parent),m_frameDelay(100),m_client(nullptr)
{

        // Preload the list of pixmaps

        // Create a timer for the timelapse
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &CameraOverview::updatePixmap);


}

void CameraOverview::load()
{
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
}

void CameraOverview::loadFirst()
{
    QStringList ls=m_client->picsFiles();
    if(!ls.isEmpty())
    {
        QPixmap ps=QPixmap(ls.last());
        if(!ps.isNull())
        {
           m_pixmaps.append(ps.scaled(size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
        }
    }


}

void CameraOverview::reset()
{
    m_counter=0;
    m_pixmaps.clear();
    m_timer->stop();
    loadFirst();
}

void CameraOverview::start()
{
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

void CameraOverview::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Toggle the timelapse
        if (m_timer->isActive()) {

            reset();
        } else {
            start();
        }
    }
}

void CameraOverview::updatePixmap()
{
    if(m_pixmaps.isEmpty())
        return;

    setPixmap(m_pixmaps[m_counter]);
    m_counter = (m_counter + 1) % m_pixmaps.size();
}

void CameraOverview::pixSlot(QString)
{
    start();
}

int CameraOverview::frameDelay() const
{
    return m_frameDelay;
}

void CameraOverview::setFrameDelay(int newFrameDelay)
{
    m_frameDelay = newFrameDelay;
}
