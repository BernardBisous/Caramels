#ifndef PARAMETEREDITOR_H
#define PARAMETEREDITOR_H

#include "Interface/namelabel.h"
#include "deviceeditor.h"
#include "growconfig.h"
#include "parameterplot.h"
#include <QWidget>

typedef enum editMode{Place, Select}EditMode;
class ParameterEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterEditor(QWidget *parent = nullptr);
    void handle(Parameter*p);
    void setXRange(int t);
    void refresh();

signals:
    void editDevice(Device* d);

private slots:
    void deviceSlot();
    void nameChangedSlot(QString);
    void pointClickedSlot(QPointF);

private:
    NameLabel* m_name;
    QLabel* m_desc;
    QLabel* m_deviceStatus;

    QWidget* m_editWidget;

    ScrollArea* m_deviceArea;


    Parameter*m_client;
    ParameterPlot* m_plot;

};

#endif // PARAMETEREDITOR_H
