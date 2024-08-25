#ifndef TENT_H
#define TENT_H

#include "growconfig.h"
#include "hardware/hardwareunit.h"
#include "qdatetime.h"
#include <QObject>



class Tent : public QObject
{
    Q_OBJECT
public:
    explicit Tent(QObject *parent = nullptr);
    void initDevices();
    void begin();

    QString configName() const;
    void setConfig(GrowConfig*e);


    HardwareUnit* unitForId(int id);
    void mapDevices();

    void addUnit(HardwareUnit* u);
    void addDevice(QList<Device*> l);
    void addDevice(Device*d);

    void restart();
    GrowConfig *config() const;

    QString name() const;
    void setName(const QString &newName);

    QList<Device *> devices() const;
    int indexOfDevice(Device*);

    int indexOf(HardwareUnit*u);
    QList<HardwareUnit *> units() const;

signals:

private:
    QList<Device*> m_devices;
    QList<HardwareUnit*> m_units;
    GrowConfig* m_config;
    QString m_configName;
    QDateTime m_startedDate;

    QString m_name;
    int m_id;
};



#endif // TENT_H
