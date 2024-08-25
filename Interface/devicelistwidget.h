#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include "Interface/scrollarea.h"
#include "hardware/device.h"
#include "hardware/hardwareunit.h"
#include <QWidget>

class DeviceListWidget : public ScrollArea
{
    Q_OBJECT
public:
    explicit DeviceListWidget(QWidget *parent = nullptr);
    void fillList(QList<Device*>l);
    void fillList(HardwareUnit*u);
    void setChecked(int i);


private slots:
    void listSlot(int, QWidget*);

signals:
    void edit(int i);


};

#endif // DEVICELISTWIDGET_H
