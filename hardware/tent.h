#ifndef TENT_H
#define TENT_H


#include "config/archive.h"
#include "config/growconfig.h"
#include "hardware/co2manager.h"
#include "hardware/hardwareunit.h"
#include "hardware/lights.h"
#include "hardware/phmanager.h"
#include "hardware/temperaturemanager.h"
#include "hardware/tolleveler.h"
#include "hardware/waterlevelmanager.h"
#include "hardware/chemicalmanager.h"
#include "hardware/generalmanager.h"

#include "hardware/webcam.h"
#include "qdatetime.h"
#include "statenotifier.h"
#include <QObject>
class Result
{
public:
    QDateTime t;
    QList<float> values;
};


class Tent : public QObject
{
    Q_OBJECT
public:
    typedef enum growingMode{undefined_mode, conventionnal, aquapony,aeropony}GrowingMode;
    typedef enum chemicalMode{undefined_chemichals, no_injection, PH_injection,Full_injection}ChemicalMode;

    explicit Tent(QObject *parent = nullptr);
    void initDevices();
    void begin();
    void stopAll();
    void restart();
    void start();
    void finish();
    void setStartDate(QDateTime t);
    void startArchive(Archive t);

    void clearAllData();
    void mapParameters();

    void addUnit(HardwareUnit* u);
    void addDevice(QList<Device*> l);
    void addDevice(Device*d);

    void saveSettings();
    void loadSetting();
    void exportAll(QString dir,bool open=true);

    int currentHourIndex();
    QString configName() const;
    void setConfig(GrowConfig*e);
    int indexOf(HardwareUnit*u);
    QList<HardwareUnit *> units() const;
    QList<HardwareUnit *> unitsForParameter(Parameter*p);
    QList<Device *> devices() const;
    QList<Actuator*> externalDevices();
    int indexOfDevice(Device*);
    QString name() const;
    void setName(const QString &newName);
    GrowConfig *config() const;
    HardwareUnit* unitForId(int id);

    float height();
    float PH();
    float temperature(int sensorIndex);
    float CO2();
    float humidity();
    float lightPower();
    float lightSpectrum();

    QDateTime startedDate() const;
    QString consoleFile();
    QString allConsole();

    QString serialPort() const;
    void setSerialPort(const QString &newSerialPort);
    QStringList availablePorts();
    bool connected();

    TemperatureManager *temperatures() const;
    PHManager* phManager();
    CO2Manager* co2Manager();
    TolLeveler *leveler() const;
    LightsUnit *lights() const;
    WaterLevelManager *pumps() const;
    Webcam *cam() const;
    ChemicalManager *chemichals() const;
    QList<ChemicalInjector*> injectors();

    void setEventsDone();

    QList<RealTimeValue> injection(int id=0);

    StateNotifier *state() const;

    void setInternalColorId(int id);
    void updateInternalColor();


    bool needsInstall();
    GrowingMode currentGrowingMode();
    void setGrowingMode(GrowingMode m);
    ChemicalMode currentChemical();
    void setChemicalMode(ChemicalMode m);


    GeneralManager *general() const;



public slots:
    void console(QString s);
    void hardwareSlot(QByteArray& d);
    void serialConnectSlot(bool s);
    void camCaptureSlot(QString);
    void errorStateSlot();
    void sendStartupEmail();

signals:
    void newValue(int i);
    void sensorsAquiered();
    void done();
    void dateChanged(QDateTime t);
    void connectedHardware(bool s);
    void consoleRequest(QString s);


private slots:
    void timerSlot();
    void tankFilledSlot(bool s);

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
    GeneralManager* m_general;
    TemperatureManager* m_temperatures;
    PHManager*m_ph;
    CO2Manager* m_Co2;
    TolLeveler* m_leveler;
    LightsUnit* m_lights;
    WaterLevelManager* m_pumps;
    ChemicalManager* m_chemichals;
    SerialTent* m_serial;
    Webcam* m_cam;
    StateNotifier* m_state;
};



#endif // TENT_H
