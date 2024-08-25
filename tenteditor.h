#ifndef TENTEDITOR_H
#define TENTEDITOR_H

#include "Interface/devicelistwidget.h"
#include "Interface/scrollarea.h"
#include "deviceeditor.h"
#include "deviceplot.h"
#include "tent.h"
#include <QWidget>

class TentEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TentEditor(QWidget *parent = nullptr);
    void handle(Tent*t);
    void edit(int index);
    void edit(HardwareUnit* s);
    int currentUnit();

public slots:
    void editDevice(int i);
signals:


private slots:
    void listSlot(int i, QWidget *);



private:
    Tent*m_client;
    ScrollArea* m_units;
    DevicePlot* m_plot;
    DeviceListWidget* m_editor;
    int m_currentUnit;

};

#endif // TENTEDITOR_H
