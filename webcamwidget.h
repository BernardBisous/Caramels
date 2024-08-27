#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H

#include "Interface/menubutton.h"
#include "Interface/switchcheckbox.h"
#include "qmediacapturesession.h"
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QCamera>
#include <QVideoWidget>

#include <QDateTime>
#include <QDir>
#include <QMediaDevices>
class WebcamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebcamWidget(QWidget *parent = nullptr) ;
    QStringList avalaible;
    QString nextName();
    static bool createDataDir();
    void setLiveMode(bool s=true);

private slots:

    void modeSlot();
    void capture();
    void enableSlot();
    void capturedSlot(int id, const QString &fileName);
    void capturingSlot();

private:
    QWidget* m_settings;
    MenuButton* m_selectMenu;
    QLabel* m_picLabel;

    QVideoWidget* m_screen;

    QCamera* m_cam;
    QImageCapture* m_capture;
    QMediaCaptureSession m_session;
    SwitchCheckBox* m_modeSwitch;
    SwitchCheckBox* m_enableSwitch;
    QTimer* m_timer;
    QString m_lastPixmap;


};




#endif // WEBCAMWIDGET_H
