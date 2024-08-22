#ifndef PARAMETEREDITOR_H
#define PARAMETEREDITOR_H

#include "Interface/namelabel.h"
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

private slots:
    void nameChangedSlot(QString);
    void pointClickedSlot(QPointF);

private:
    NameLabel* m_name;



    QWidget* m_editWidget;


    Parameter*m_client;
    ParameterPlot* m_plot;

};

#endif // PARAMETEREDITOR_H
