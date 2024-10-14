#ifndef PARAMETERVALUEEDITOR_H
#define PARAMETERVALUEEDITOR_H

#include "qlabel.h"
#include "qlineedit.h"
#include <QWidget>

#include "Interface/toolbutton.h"
#include "config/parameter.h"
#include "widgets/parameterplot.h"


class ParameterValueEditor : public QWidget
{
    Q_OBJECT
public:
    typedef enum mode{modeGain=0,modeOffset,modeAlone}Mode;
    explicit ParameterValueEditor(QWidget *parent = nullptr);

    Parameter *client() const;
    void setClient(Parameter *newClient);


    void refreshPlot();
    void refresh();
    ToolButton *addMode(QString icon,QString title);
    Mode mode() const;
    void setMode(Mode newMode);
    void refreshMode();
    void move(int inc);
    void move (float val);
    void edit(QList<int>indexes );
    void edit(int i);

    void setGain(float newGain);

    void setOffset(float newOffset);
    void selectOffset(int v);

    void setPlot(ParameterPlot* p);

    void refreshValue();
    int lastSelected();

    float currentValue(bool *ok);

signals:
    void changed();



public slots:
    void modeEdited();
    void seriesSelected();
    void valueTyped();
    void up();
    void down();
    void left();
    void right();

private:
    Parameter* m_client;
    QList<ToolButton*> m_modes;
    Mode m_mode;


    ToolButton* m_up;
    ToolButton* m_down;
    ToolButton* m_left;
    ToolButton* m_right;

    QLineEdit*m_valueEdit;

    QList<int>m_indexes;
    QLabel* m_value;
    QLabel*m_name;
    QLabel*m_description;
    QLabel*m_units;



    ParameterPlot*m_plot;


    float m_gain;

};

#endif // PARAMETERVALUEEDITOR_H
