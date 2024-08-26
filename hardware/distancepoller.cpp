#include "distancepoller.h"
#include <QThread>
#include "qdatetime.h"
#include "rasppi.h"


DistancePoller::DistancePoller(QObject *parent)
    : QObject{parent},m_thread(nullptr)
{

    m_thread = new QThread;
    moveToThread(m_thread);

}

void DistancePoller::pollSensor(int n)
{
    emit distanceMeasured(50);
    /*
    m_thread->start();
    float out=0;

    int i=0;
    while (i<n) {


        float v=calculateDistance();

        if(v>0)
        {
            out+=v;
            i++;
            m_thread->msleep(10);
        }

    }




    if(i)
        out=out/i;

    emit distanceMeasured(out);
    */

}
float DistancePoller::calculateDistance() {
    // Replace with your specific GPIO pin configuration and timing calculations
    // Assuming you have GPIO pins for trigger and echo, and appropriate timing constants


    // Trigger the sensor

    QElapsedTimer t;


    RasPi::write(m_trigPin,100);
    m_thread->usleep(10); // Short delay to ensure trigger pulse is stable
    RasPi::write(m_trigPin,0);


    while (!RasPi::readHigh(m_Echopin)) {
        if(t.elapsed()>50)
        {
            qDebug()<<"No distance felt"<<t.elapsed();
            return-1;
        }
    }

    int elapsed=t.nsecsElapsed();



    double elapsedTimeSeconds = elapsed / 1000000000;



       // Calculate the distance based on the elapsed time and speed of sound
       // Adjust the speed of sound value based on your specific environment
    const float speedOfSound = 343.0f; // Speed of sound in meters per second at 20 degrees Celsius
    int distance = (elapsedTimeSeconds * speedOfSound) / 2 * 100; // Convert to centimeters

   // qDebug()<<"measuring sensor"<<elapsed<<distance;
    return distance;

}

void DistancePoller::startPolling(int echo, int trig, int n)
{
    //qDebug()<<"polling sensor"<<n;

    m_trigPin=trig;
    m_Echopin=echo;


    pollSensor(n);
}
