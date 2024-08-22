#ifndef LIGHTS_H
#define LIGHTS_H


#include "hardware/device.h"

class Lights: public Device
{
    Q_OBJECT
public:
    explicit Lights(QObject *parent = nullptr);
};

#endif // LIGHTS_H
