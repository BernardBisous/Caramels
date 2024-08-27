#ifndef ROUNDEDPICTURE_H
#define ROUNDEDPICTURE_H

#include "qlabel.h"
#include <QWidget>

class RoundedPicture : public QLabel
{
    Q_OBJECT
public:
    explicit RoundedPicture(int radius=10,QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    int radius() const;
    void setRadius(int newRadius);

signals:

private:
    int m_radius;


};

#endif // ROUNDEDPICTURE_H
