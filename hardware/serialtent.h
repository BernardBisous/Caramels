#ifndef SERIALTENT_H
#define SERIALTENT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialTent : public QObject
{
    Q_OBJECT
public:
    SerialTent(QObject *parent = nullptr);
    ~SerialTent();

    void sendValues();
    bool isConnected() const;
    void write(int pin, float value);
    float read(int pin);

    QStringList forbiden() const;

public slots:
    bool open(const QString &portName);
    void close();
    void retry();
    void errorSlot(QSerialPort::SerialPortError);

signals:
    void newValues(QByteArray &values);
    void connectedChanged(bool s);

private:
    QSerialPort serialPort;
    QByteArray configBytes;
    QByteArray values;

    QHash<int,int> m_input;
    QHash<int,int> m_outputPins;

    QStringList m_forbiden;
    void readSerial();
};




#endif // SERIALTENT_H
