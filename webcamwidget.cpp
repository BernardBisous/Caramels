#include "webcamwidget.h"
#include "qboxlayout.h"

#include <QMediaCaptureSession>
#include <QImageCapture>
WebcamWidget::WebcamWidget(QWidget *parent):QWidget(parent),m_cam(nullptr),m_session()
{


    setLayout(new QHBoxLayout);
    layout()->addWidget(m_screen=new QVideoWidget);
    m_screen->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout()->addWidget(m_settings=new QWidget);
    m_settings->setLayout(new QVBoxLayout);
    m_settings->layout()->addWidget(m_selectMenu=new MenuButton);
    m_settings->layout()->addWidget(m_lightSlider=new Slider);
    m_lightSlider->setOrientation(Qt::Horizontal);
    m_lightSlider->setMaximum(100);
    m_lightSlider->setMinimum(0);
    connect(m_lightSlider,SIGNAL(valueChanged(int)),this,SLOT(lightSlot(int)));


    m_capture=new QImageCapture;
    m_session.setImageCapture(m_capture);
    m_session.setVideoOutput(m_screen);

    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras)
        m_selectMenu->nouvAction(cameraDevice.description());


    if(cameras.count()>=1)
    {

        m_cam = new QCamera(cameras.first());

        m_selectMenu->setActivate(m_cam->cameraDevice().description());

        m_session.setCamera(m_cam);


        m_screen->show();
        m_cam->start();

    }

    capture();
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(capture()));

    m_timer->start(1000*3600);
}

QString WebcamWidget::nextName()
{

    QString baseFilename="photo";
    QString dirPath="pics";
    QString filename = baseFilename;
    int i = 1;

    while (QFile::exists(dirPath + "/" + filename)) {
        filename = baseFilename + QString::number(i++);
    }

    return filename;
}

void WebcamWidget::capture()
{

    m_capture->captureToFile(nextName());
}


