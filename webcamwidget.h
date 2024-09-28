#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H

#include "Interface/menubutton.h"
#include "Interface/switchcheckbox.h"
#include "Interface/toolbutton.h"
#include "hardware/webcam.h"
#include "qmediacapturesession.h"
#include "qstackedwidget.h"
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QCamera>
#include <QVideoWidget>

#include <QDateTime>
#include <QDir>
#include <QMediaDevices>
#include <webcamoverview.h>

class WebcamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebcamWidget(QWidget *parent = nullptr) ;
    void handle(Webcam* w);
    void printPicture(QString s);
    void printPicture(QPixmap p);
    static bool createDataDir();
    void setLiveMode(bool s=true);

private slots:
    void modeSlot();
    void enableSlot();
    void capturedSlot(QString fileName);
    void findCam();
    void timelapse();
    void capture();

private:
    QWidget* m_settings;
    MenuButton* m_selectMenu;
    CameraOverview* m_picLabel;
    QVideoWidget* m_screen;

    SwitchCheckBox* m_modeSwitch;
    SwitchCheckBox* m_enableSwitch;
    ToolButton * m_capture;


    ToolButton* m_finder;
    SwitchCheckBox* m_timeLaspe;



    Webcam* m_client;

    QStackedWidget* m_stack;

};




#endif // WEBCAMWIDGET_H
