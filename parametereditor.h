#ifndef PARAMETEREDITOR_H
#define PARAMETEREDITOR_H

#include "Interface/namelabel.h"
#include "deviceeditor.h"
#include "growconfig.h"
#include "parameterplot.h"
#include "parametervalueeditor.h"
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
    void edit(int i);

    int currentIndex() const;
    void setCurrentIndex(int newCurrentIndex);

signals:
    void editDevice(Device* d);
    void edited();

private slots:
    void deviceSlot();
    void nameChangedSlot(QString);
    void pointClickedSlot(QPointF);


    void editSlot();

private:
    NameLabel* m_name;
    QLabel* m_desc;


    QWidget* m_editWidget;


    ParameterValueEditor* m_editor;
    Parameter*m_client;
    ParameterPlot* m_plot;

    int m_currentIndex;

};

#endif // PARAMETEREDITOR_H
