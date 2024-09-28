#ifndef WEBCAMOVERVIEW_H
#define WEBCAMOVERVIEW_H


#include "hardware/webcam.h"
#include "qlabel.h"


class CameraOverview : public QLabel
{
    Q_OBJECT
public:
    explicit CameraOverview(QWidget *parent = nullptr);

    void load();
    void loadFirst();
    void reset();
    void start();
    void handle(Webcam* w);

    int frameDelay() const;
    void setFrameDelay(int newFrameDelay);

    void resizeEvent(QResizeEvent *event) override;
    void printPixmap();


public slots:
     void updatePixmap();
     void pixSlot(QString);

private:
    int m_counter;
    int m_frameDelay;
    Webcam* m_client;
    QTimer *m_timer;
    QPixmap m_pix;
    QStringList m_pixNames;
};

#endif // WEBCAMOVERVIEW_H
