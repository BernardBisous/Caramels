#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H

#include "Interface/menubutton.h"
#include "Interface/slider.h"
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

private slots:
    void lightSlot(int);
    void capture();

private:
    QWidget* m_settings;
    MenuButton* m_selectMenu;
    QVideoWidget* m_screen;
    Slider * m_lightSlider;
    Slider * m_zoomSlider;
    QCamera* m_cam;
    QImageCapture* m_capture;
    QMediaCaptureSession m_session;
    QTimer* m_timer;


};




#endif // WEBCAMWIDGET_H
