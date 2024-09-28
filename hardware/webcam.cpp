#include "webcam.h"
#include "qtimer.h"


#include <QMediaDevices>
#include <QDir>
#include <QFile>

#define DATA_DIR "pics"
#define CAPTURE_DELAY_HOURS 3
#define STARTUP_DELAY 1500
#include "parameter.h"

Webcam::Webcam(QObject *parent)
    : QObject{parent},m_cam(nullptr),
      m_session(),m_lastPixmap(),m_accessible(true)
{
    m_capture=new QImageCapture;
    connect(m_capture,SIGNAL(imageSaved(int,QString)),this,SLOT(capturedSlot(int,QString)));
    createDataDir();
    findCam();
}

bool Webcam::shouldCapture()
{

    if(!m_nextDate.isValid())
        return true;

    return m_nextDate<QDateTime::currentDateTime();
}

void Webcam::setEnabled(bool s)
{
    if(s)
    {
        start();

    }
    else stop();
}

bool Webcam::createDataDir()
{

    QString sp(DATA_DIR);

    QDir r(sp);

    if(!r.exists())
    {
        QDir d=QDir::current();
        d.mkdir(sp);
        return true;
    }
    return false;
}

QString Webcam::dataDir()
{
    return DATA_DIR;
}

QMediaCaptureSession *Webcam::session()
{
    return &m_session;
}

void Webcam::startLive()
{
    if(!m_cam)
        return;

    stop();
    m_cam->start();
}

QStringList Webcam::picsFiles()
{
    QDir dir(DATA_DIR);
    auto l=dir.entryList(QStringList() << "*.jpg", QDir::Files | QDir::NoDot);
    QStringList out;
    for(int i=0;i<l.count();i++)
        out<<dir.absoluteFilePath(l[i]);
    return  out;
}

void Webcam::clearAll()
{
    QDir dir(DATA_DIR);
    auto l=picsFiles();
    for(int i=0;i<l.count();i++)
    {
        QFile::remove(l[i]);
    }
}

void Webcam::exportAll(QString dir)
{
    auto l=picsFiles();
    for(int i=0;i<l.count();i++)
    {
        QFile::copy(l[i],dir+"/pict"+QString::number(i)+".jpg");
    }
}

void Webcam::scheduleNext()
{
    m_nextDate=QDateTime::currentDateTime().addSecs(CAPTURE_DELAY_HOURS*Parameter::timeMultiplicator());
    m_nextDate=QDateTime::currentDateTime().addSecs(5);

}

QString Webcam::diagnose()
{
    if(!m_cam)
        return "Pas de webcam trouvée";

    if(!m_accessible)
         return "Camera inacessible";

    return "";
}



void Webcam::capture()
{
    if(!m_cam)
        return;

    m_cam->start();
    QTimer::singleShot(STARTUP_DELAY,this,SLOT(capturingSlot()));

}

void Webcam::findCam()
{
    if(m_cam)
        return;

    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

    if(cameras.count()>=1)
    {
        m_cam = new QCamera(cameras.first());
        m_session.setCamera(m_cam);
    }
}

QString Webcam::nextName()
{

    QString baseFilename="photo";
    QString dirPath=QDir::currentPath()+"/"+DATA_DIR;
    QString filename = baseFilename+".jpg";
    int i = 1;

    while (QFile::exists(dirPath + "/" + filename)) {
        filename = baseFilename + QString::number(i++)+".jpg";
    }

    return dirPath + "/"+filename;
}

void Webcam::capturingSlot()
{

    QString s=nextName();
    m_lastPixmap=s;
    m_capture->captureToFile(s);
    m_cam->stop();
    scheduleNext();

}

void Webcam::start()
{
    m_session.setImageCapture(m_capture);
    capture();

}

void Webcam::stop()
{

  // m_cam->stop();
}

QString Webcam::name()
{
    if(!m_cam)
        return "";

    return m_cam->cameraDevice().description();
}

QStringList Webcam::availables()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    QStringList out;
    for(int i=0;i<cameras.count();i++)
        out<<cameras[i].description();

    return out;
}

void Webcam::capturedSlot(int, QString s)
{
    emit saved(s);
}

bool Webcam::accessible() const
{
    return m_accessible;
}

CamState::CamState(Webcam *c, QObject *parent):
    DeviceState(nullptr,nullptr,parent),m_cam(c)
{

}

QString CamState::diagnosis() const
{
    return m_cam->diagnose();
}

QString CamState::name()
{
    return m_cam->name();
}



void CamState::refresh()
{
    clientError(diagnosis(),false);
}
