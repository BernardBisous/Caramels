#ifndef WINDMANAGER_H
#define WINDMANAGER_H

#include "hardware/hardwareunit.h"
#include <QObject>

class WindManager : public HardwareUnit
{
    Q_OBJECT
public:
    explicit WindManager(QObject *parent = nullptr);


signals:

private:
     SwitchedActuator* m_power;
      SwitchedActuator* m_rotation;

};

#endif // WINDMANAGER_H
