#ifndef CONFIGEDITOR_H
#define CONFIGEDITOR_H

#include "parametereditor.h"
#include "parameterlistwidget.h"
#include "tent.h"
#include <QWidget>

class ConfigEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigEditor(QWidget *parent = nullptr);
    void handle(GrowConfig* t);





public slots:
    void loadData();
    void addSlot();
    void editParameter(int i);
    void editParameter(Parameter*p);
    void editDevicePropagate(Device*d){emit editDevice(d);}
    void setCurrentIndex(int newCurrentIndex);
    void saveConfig();

signals:
    void editDevice(Device* d);
    ;

private:
    GrowConfig* m_client;
    ParameterEditor* m_editor;
    ParameterListWidget* m_list;
    int m_currentIndex;
};

#endif // CONFIGEDITOR_H
