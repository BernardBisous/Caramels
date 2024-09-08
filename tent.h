#ifndef TENT_H
#define TENT_H

#include "growconfig.h"
#include "hardware/co2manager.h"
#include "hardware/hardwareunit.h"
#include "hardware/lights.h"
#include "hardware/phmanager.h"
#include "hardware/temperaturemanager.h"
#include "hardware/tolleveler.h"
#include "hardware/waterlevelmanager.h"
#include "hardware/webcam.h"
#include "qdatetime.h"
#include <QObject>



class Tent : public QObject
{
    Q_OBJECT
public:
    explicit Tent(QObject *parent = nullptr);
    void initDevices();
    void begin();

    void exportAll(QString dir);

    int currentHourIndex();
    QString configName() const;
    void setConfig(GrowConfig*e);


    HardwareUnit* unitForId(int id);
    void mapDevices();

    void addUnit(HardwareUnit* u);
    void addDevice(QList<Device*> l);
    void addDevice(Device*d);

    void saveSettings();
    void loadSetting();

    void restart();
    void start();
    void setStartDate(QDateTime t);
    GrowConfig *config() const;

    QString name() const;
    void setName(const QString &newName);

    QList<Device *> devices() const;
    int indexOfDevice(Device*);

    int indexOf(HardwareUnit*u);
    QList<HardwareUnit *> units() const;

    float PH();
    float temperature(int sensorIndex);
    float CO2();
    float humidity();
    float lightPower();
    float lightSpectrum();
    QString injectingState();

    QDateTime startedDate() const;
    QString consoleFile();

    void finish();

    QString serialPort() const;
    void setSerialPort(const QString &newSerialPort);
    QStringList availablePorts();
    bool connected();

    void stopAll();

    TemperatureManager *temperatures() const;
    PHManager* phManager();
    CO2Manager* co2Manager();

    TolLeveler *leveler() const;


    LightsUnit *lights() const;

    WaterLevelManager *pumps() const;

    Webcam *cam() const;

public slots:
    void console(QString s);
    void hardwareSlot(QByteArray& d);
    void serialConnectSlot(bool s);

signals:
    void newValue(int i);
    void sensorsAquiered();
    void done();
    void dateChanged(QDateTime t);
    void connectedHardware(bool s);

private slots:
    void timerSlot();


private:
    QList<Device*> m_devices;
    QList<HardwareUnit*> m_units;
    GrowConfig* m_config;
    QString m_configName;
    QDateTime m_startedDate;
    QString m_name;
    int m_id;
    QString m_serialPort;

    QTimer* m_timer;

    TemperatureManager* m_temperatures;
    PHManager*m_ph;
    CO2Manager* m_Co2;

    TolLeveler* m_leveler;
    LightsUnit* m_lights;
    WaterLevelManager* m_pumps;

    SerialTent* m_serial;

    Webcam* m_cam;
};



#endif // TENT_H
