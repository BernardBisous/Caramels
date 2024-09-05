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




public slots:
    bool open(const QString &portName);
    void close();
    void retry();

signals:
    void newValues(QByteArray &values);



private:
    QSerialPort serialPort;
    bool connected;
    QByteArray configBytes;
    QByteArray values;

    QHash<int,int> m_input;
    QHash<int,int> m_outputPins;

    void readSerial();
};

#endif // SERIALTENT_H
