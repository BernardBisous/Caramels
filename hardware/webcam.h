#ifndef WEBCAM_H
#define WEBCAM_H

#include "qcamera.h"
#include "qdatetime.h"
#include "qimagecapture.h"
#include "qmediacapturesession.h"
#include "qpixmap.h"
#include "statenotifier.h"
#include <QObject>

class Webcam : public QObject
{
    Q_OBJECT
public:
    explicit Webcam(QObject *parent = nullptr);
    bool shouldCapture();

    void setEnabled(bool s=true);
     static bool createDataDir();
     static QString dataDir();
    QMediaCaptureSession* session();
    void startLive();
    QStringList picsFiles();
    void clearAll();
    void exportAll(QString dir);
    void scheduleNext();
    QString diagnose();
    bool accessible() const;

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
    QDateTime m_nextDate;
    QCamera* m_cam;
    QImageCapture* m_capture;
    QMediaCaptureSession m_session;
    QPixmap m_lastPixmap;
    bool m_accessible;

};



class CamState:  public DeviceState
{
    Q_OBJECT
public:
    explicit CamState(Webcam* c,QObject*parent);
    virtual QString diagnosis() const;
    virtual QString name();
    virtual void refresh();

private:
    Webcam* m_cam;

};

#endif // WEBCAM_H
