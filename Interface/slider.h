#ifndef SLIDER_H
#define SLIDER_H

#include "qslider.h"
#include <QWidget>

class Slider : public QSlider {
public:
    explicit Slider(QWidget *parent = nullptr) ;

protected:
    void paintEvent(QPaintEvent *event) override ;
};

#endif // SLIDER_H
