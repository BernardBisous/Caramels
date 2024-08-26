#ifndef RASPPI_H
#define RASPPI_H

#include <QObject>

class RasPi : public QObject
{
    Q_OBJECT
public:
    typedef enum pinMode{INPUT,OUTPUT,INPUT_PULL}PinMode;
    explicit RasPi(QObject *parent = nullptr);
    static void initPin(int pin, PinMode m);
    static void write(int pin, int purcent);
    static bool readHigh(int pin);
    static int  readAnalog(int pin);
    static void begin();


signals:

};

#endif // RASPPI_H
