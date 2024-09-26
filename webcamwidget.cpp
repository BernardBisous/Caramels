#include "webcamwidget.h"
#include "qboxlayout.h"

#include <QMediaCaptureSession>
#include <QImageCapture>

WebcamWidget::WebcamWidget(QWidget *parent):QWidget(parent),m_client(nullptr)
{
    setLayout(new QHBoxLayout);
    layout()->addWidget(m_screen=new QVideoWidget);

    layout()->addWidget(m_picLabel=new CameraOverview);

    m_screen->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    m_settings=new QWidget(this);
    m_settings->setLayout(new QVBoxLayout);
    m_settings->layout()->addWidget(m_finder=new ToolButton("Find cam"));
    m_settings->layout()->addWidget(m_selectMenu=new MenuButton);
    m_settings->layout()->addWidget(m_enableSwitch=new SwitchCheckBox("Enable"));
    m_settings->layout()->addWidget(m_modeSwitch=new SwitchCheckBox("Live mode"));
    m_settings->layout()->addWidget(m_timeLaspe=new SwitchCheckBox("Timelapse"));
    m_settings->show();


    layout()->addWidget(m_settings);


    connect(m_modeSwitch,SIGNAL(stateChanged(int)),this,SLOT(modeSlot()));
    connect(m_enableSwitch,SIGNAL(stateChanged(int)),this,SLOT(enableSlot()));
    connect(m_timeLaspe,SIGNAL(stateChanged(int)),this,SLOT(timelapse()));
    connect(m_finder,SIGNAL(clicked()),this,SLOT(findCam()));
    connect(m_timeLaspe,SIGNAL(clicked()),this,SLOT(timelapse()));

    m_lapseTimer=new QTimer(this);
    m_lapseTimer->setInterval(100);
    connect(m_lapseTimer,SIGNAL(timeout()),this,SLOT(lapsSlot()));
    setLiveMode(false);
    m_enableSwitch->setChecked(true);
}

void WebcamWidget::handle(Webcam *w)
{
    m_client=w;
    findCam();


    m_picLabel->handle(w);
    m_screen->hide();


    connect(w,SIGNAL(saved(QString)),this,SLOT(capturedSlot(QString)));
}

void WebcamWidget::printPicture(QString s)
{
    printPicture(QPixmap(s));

}

void WebcamWidget::printPicture(QPixmap p)
{
  //  if(!p.isNull())
  //      m_picLabel->setPixmap(p.scaled(m_picLabel->size()-QSize(30,30),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void WebcamWidget::setLiveMode(bool s)
{
    if(!m_client)
        return;

    m_modeSwitch->setChecked(s);
    m_screen->setHidden(!s);
    m_picLabel->setHidden(s);

    if(s)
    {
        m_client->session()->setVideoOutput(m_screen);
        m_client->startLive();

       // layout()->addWidget(m_settings);

    }

    else
    {


      //  m_settings->setGeometry(20,20,200,200);
       // m_settings->show();

        m_settings->raise();
        m_settings->show();
        m_client->start();
    }
}

void WebcamWidget::resetTimeLapse()
{

    QStringList ls=m_client->picsFiles();
    m_lapsFrames.clear();
    for(int i=0;i<ls.count();i++)
    {
        m_lapsFrames<<QPixmap(ls[i]);
    }

    m_lapsCounter=0;
}

void WebcamWidget::resizeEvent(QResizeEvent *e)
{

    QWidget::resizeEvent(e);


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
    if(!m_lapseTimer->isActive())
       printPicture(fileName);
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

    if(!m_timeLaspe->isChecked())
    {
        m_lapseTimer->stop();
        return;
    }
    setLiveMode(false);
    resetTimeLapse();
    m_lapseTimer->start();
}

void WebcamWidget::lapsSlot()
{
    if(m_lapsCounter>=m_lapsFrames.count())
    {
       m_lapsCounter=0;
    }
    printPicture(m_lapsFrames[m_lapsCounter]);
}


