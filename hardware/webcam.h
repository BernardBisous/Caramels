#ifndef WEBCAM_H
#define WEBCAM_H

#include "qcamera.h"
#include "qimagecapture.h"
#include "qmediacapturesession.h"
#include "qpixmap.h"
#include <QObject>

class Webcam : public QObject
{
    Q_OBJECT
public:
    explicit Webcam(QObject *parent = nullptr);
    void setEnabled(bool s=true);
     static bool createDataDir();
     static QString dataDir();
    QMediaCaptureSession* session();
    void startLive();
    QStringList picsFiles();
    void clearAll();
    void exportAll(QString dir);

public slots:
    void capture();
    void findCam();
    QString nextName();
    void capturingSlot();
    void start();
    void stop();
    QString name();
    QStringList availables();
    void capturedSlot(int,QString s);


signals:
    void saved(QString s);

private:
    QCamera* m_cam;
    QImageCapture* m_capture;
    QMediaCaptureSession m_session;
    QPixmap m_lastPixmap;
};

#endif // WEBCAM_H
