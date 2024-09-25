#include "serialtent.h"
#include <QTimer>


SerialTent::SerialTent(QObject *parent) :
    QObject(parent),
    serialPort(this)
{

    m_forbiden=QStringList()<<"COM1";
    connect(&serialPort, &QSerialPort::readyRead, this, &SerialTent::readSerial);
    connect(&serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(errorSlot(QSerialPort::SerialPortError)));
}

SerialTent::~SerialTent()
{
    close();
}

bool SerialTent::open(const QString &portName)
{
    if (isConnected()) {
        return true;
    }


    if(portName.isEmpty())
        return false;

    serialPort.setPortName(portName);
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort.open(QIODevice::ReadWrite))
    {
        QTimer::singleShot(1000,this,SLOT(retry()));
        return false;
    }


    m_input.clear();

    emit connectedChanged(true);
    return true;
}

void SerialTent::close()
{
    if (isConnected()) {
        serialPort.close();
    }
}

void SerialTent::retry()
{

    if(isConnected())
        return;



    if (!serialPort.open(QIODevice::ReadWrite))
    {
        QTimer::singleShot(1000,this,SLOT(retry()));
    }
    else
    {

        emit connectedChanged(true);
    }
}

void SerialTent::errorSlot(QSerialPort::SerialPortError c)
{

    if(c==QSerialPort::PermissionError)
    {
        //qDebug()<<"Lost serial connection"<<c;

        serialPort.close();
        QTimer::singleShot(1000,this,SLOT(retry()));
        emit connectedChanged(false);
    }


}

QStringList SerialTent::forbiden() const
{
    return m_forbiden;
}

void SerialTent::sendValues()
{

}

bool SerialTent::isConnected() const
{
    return serialPort.isOpen();
}

void SerialTent::write(int pin, float value)
{

    if(pin<0 || !serialPort.isOpen())
        return;

    quint8 v=value*2.55;
    QByteArray out;
    out.append((char)pin);
    out.append((char)v);
    out.append('\n');

   // qDebug()<<"writeing"<<pin<<value;
    serialPort.write(out);

}

float SerialTent::read(int pin)
{
    if(!m_input.contains(pin) || pin<=0)
        return -1;

    float v=m_input.value(pin);
    v=v/2.55;
    return v;
}

void SerialTent::readSerial()
{
    if (!serialPort.isOpen()) {
        emit connectedChanged(false);
        return;
    }
    QByteArray data = serialPort.readAll();
    if (data.contains("\n")) {
        values = data.trimmed();
        for(int i=0;i<data.size();i++)
        {
            if(i%2)
            {
                int key=data.at(i-1);
                int val=quint8(data.at(i));
                m_input.insert(key,val);
               // qDebug()<<"read values"<<key<<val;
            }
        }
        emit newValues(data);
    }
}

