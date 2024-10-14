#include "webcamwidget.h"
#include "qboxlayout.h"

#include <QMediaCaptureSession>
#include <QImageCapture>

WebcamWidget::WebcamWidget(QWidget *parent):QWidget(parent),m_client(nullptr)
{
    setLayout(new QHBoxLayout);
    layout()->addWidget(m_stack=new QStackedWidget);
    m_stack->addWidget(m_screen=new QVideoWidget);
    m_stack->addWidget(m_picLabel=new CameraOverview);
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(0);

    m_stack->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    m_settings=new QWidget;
    m_settings->setLayout(new QVBoxLayout);
    QWidget*ws=new QWidget;
    ws->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_settings->layout()->addWidget(ws);

    m_settings->layout()->addWidget(m_finder=new ToolButton("Find cam"));
    m_settings->layout()->addWidget(m_selectMenu=new MenuButton);
    m_settings->layout()->addWidget(m_enableSwitch=new SwitchCheckBox("Enable"));
    m_settings->layout()->addWidget(m_modeSwitch=new SwitchCheckBox("Live mode"));
    m_settings->layout()->addWidget(m_timeLaspe=new SwitchCheckBox("Timelapse"));
    m_settings->layout()->addWidget(m_capture=new ToolButton("Capture"));
    m_settings->setFixedWidth(150);




    layout()->addWidget(m_settings);


    connect(m_modeSwitch,SIGNAL(stateChanged(int)),this,SLOT(modeSlot()));
    connect(m_enableSwitch,SIGNAL(stateChanged(int)),this,SLOT(enableSlot()));
    connect(m_timeLaspe,SIGNAL(stateChanged(int)),this,SLOT(timelapse()));
    connect(m_finder,SIGNAL(clicked()),this,SLOT(findCam()));
    connect(m_timeLaspe,SIGNAL(clicked()),this,SLOT(timelapse()));
    connect(m_capture,SIGNAL(clicked()),this,SLOT(capture()));


    m_enableSwitch->setChecked(true);
    m_enableSwitch->setHidden(true);

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);
}

void WebcamWidget::handle(Webcam *w)
{
    m_client=w;
    findCam();
    m_picLabel->handle(w);

    connect(w,SIGNAL(saved(QString)),this,SLOT(capturedSlot(QString)));
    setLiveMode(false);
}

void WebcamWidget::printPicture(QString s)
{
}

void WebcamWidget::printPicture(QPixmap p)
{
}

void WebcamWidget::setLiveMode(bool s)
{
    if(!m_client)
        return;


    m_modeSwitch->setChecked(s);

    if(s)
    {
        m_client->session()->setVideoOutput(m_screen);
        m_client->startLive();
        m_stack->setCurrentWidget(m_screen);
    }

    else
    {
        m_stack->setCurrentWidget(m_picLabel);

        if(m_timeLaspe->isChecked())
            m_picLabel->start();
        else
            m_picLabel->reset();
    }
}





void WebcamWidget::modeSlot()
{
    setLiveMode(m_modeSwitch->isChecked());
}

void WebcamWidget::enableSlot()
{
    if(m_client)
        m_client->setEnabled(m_enableSwitch->isChecked());
}

void WebcamWidget::capturedSlot(QString fileName)
{

}

void WebcamWidget::findCam()
{
    m_selectMenu->clear();
    m_client->findCam();
    m_selectMenu->addActions(m_client->availables());
    m_selectMenu->setActivate(m_client->name());
}

void WebcamWidget::timelapse()
{
    if(m_timeLaspe->isChecked())
        m_picLabel->start();
    else
        m_picLabel->reset();
}



void WebcamWidget::capture()
{
    m_client->capture();
}


