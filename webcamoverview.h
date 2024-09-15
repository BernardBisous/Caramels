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

protected:
    void mousePressEvent(QMouseEvent *event);

public slots:
     void updatePixmap();
private:


    int m_counter;
    int m_frameDelay;
    Webcam* m_client;
    QList<QPixmap> m_pixmaps;
    QTimer *m_timer;
};

#endif // WEBCAMOVERVIEW_H
