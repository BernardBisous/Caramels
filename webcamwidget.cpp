#include "webcamwidget.h"
#include "qboxlayout.h"

#include <QMediaCaptureSession>
#include <QImageCapture>

#define CAPTURE_DELAY 5000
WebcamWidget::WebcamWidget(QWidget *parent):QWidget(parent),m_cam(nullptr),m_session()
{

    createDataDir();

    setLayout(new QHBoxLayout);
    layout()->addWidget(m_screen=new QVideoWidget);
    layout()->addWidget(m_picLabel=new QLabel);
    m_screen->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout()->addWidget(m_settings=new QWidget);
    m_settings->setLayout(new QVBoxLayout);
    m_settings->layout()->addWidget(m_selectMenu=new MenuButton);
    m_settings->layout()->addWidget(m_enableSwitch=new SwitchCheckBox("Enable"));
    m_settings->layout()->addWidget(m_modeSwitch=new SwitchCheckBox("Live mode"));

    m_settings->setFixedWidth(200);
    QWidget* sp=new QWidget;
    sp->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    m_settings->layout()->addWidget(sp);

    connect(m_modeSwitch,SIGNAL(stateChanged(int)),this,SLOT(modeSlot()));
    connect(m_enableSwitch,SIGNAL(stateChanged(int)),this,SLOT(enableSlot()));

    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras)
        m_selectMenu->nouvAction(cameraDevice.description());


    m_capture=new QImageCapture;

    if(cameras.count()>=1)
    {
        m_cam = new QCamera(cameras.first());
        m_selectMenu->setActivate(m_cam->cameraDevice().description());
         m_session.setCamera(m_cam);

    }


    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(capture()));
    connect(m_capture,SIGNAL(imageSaved(int,QString)),this,SLOT(capturedSlot(int,QString)));




    setLiveMode(false);


}

QString WebcamWidget::nextName()
{

    QString baseFilename="photo";
    QString dirPath=QDir::currentPath()+"/pics";
    QString filename = baseFilename+".jpg";
    int i = 1;

    while (QFile::exists(dirPath + "/" + filename)) {
        filename = baseFilename + QString::number(i++)+".jpg";
    }

    return dirPath + "/"+filename;
}

bool WebcamWidget::createDataDir()
{
    QString sp("pics");

    QDir r(sp);

    if(!r.exists())
    {
        QDir d=QDir::current();
        d.mkdir(sp);
        return true;
    }
    return false;
}

void WebcamWidget::setLiveMode(bool s)
{
    if(!m_cam)
        return;

    m_modeSwitch->setChecked(s);
    m_screen->setHidden(!s);
    m_picLabel->setHidden(s);

    if(s)
    {
        m_timer->stop();
        m_session.setVideoOutput(m_screen);
        m_cam->start();
    }


    else
    {
        m_session.setImageCapture(m_capture);
        m_timer->start(CAPTURE_DELAY);
        capture();
    }
}



void WebcamWidget::modeSlot()
{
    qDebug()<<"mode webcam";
    setLiveMode(m_modeSwitch->isChecked());

}

void WebcamWidget::capture()
{

    m_cam->start();
    QTimer::singleShot(1000,this,SLOT(capturingSlot()));
}

void WebcamWidget::enableSlot()
{
    qDebug()<<"enabling webcam";
    if(m_enableSwitch->isChecked())
    {
        qDebug()<<"enabling webcam";
        m_cam->start();
        capture();
    }


    else
        m_cam->stop();
}

void WebcamWidget::capturedSlot(int , const QString &fileName)
{
     qDebug()<<"captrue picmap"<<fileName;
     m_picLabel->setPixmap(QPixmap(fileName).scaled(m_picLabel->size()-QSize(30,30),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void WebcamWidget::capturingSlot()
{
    QString s=nextName();
    m_lastPixmap=s;
    m_capture->captureToFile(s);
    m_cam->stop();
}


