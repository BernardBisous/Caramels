#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "qdatetime.h"
#include <QProgressBar>

class ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = nullptr);
    void setCountDown(int ms,float val=0);

    float value() const;
    void setValue(double newValue);

protected:
  void paintEvent(QPaintEvent *event) override;

public slots:
  void timerSlot();

signals:
    void timeout();

private:
   QTimer* m_timer;
   QTime m_startTime;
   int m_maxCount;
   int m_count;
   double m_value;

};

#endif // PROGRESSBAR_H
