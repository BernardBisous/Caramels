#ifndef STARTWIZZARD_H
#define STARTWIZZARD_H

#include "hardware/tent.h"
#include "widgets/archivewidget.h"
#include "widgets/configeditor.h"
#include "wizzard.h"
#include <QWidget>


class ConfigEditorSequence: public WizzardSequence
{
    Q_OBJECT
public:
    explicit ConfigEditorSequence(Tent* t,QWidget *parent = nullptr);
    void start();


private:
    ConfigEditor* m_config;
    Tent*m_tent;
};

class ConfigWizzardSequence: public WizzardSequence
{
    Q_OBJECT
public:
    explicit ConfigWizzardSequence(Tent* t,QWidget *parent = nullptr);

private slots:
    void reuseSlot(Archive a);


private:

    ArchiveWidget* m_archives;

    Tent*m_tent;
};

class StartWizzard:  public Wizzard
{
    Q_OBJECT
public:
    explicit StartWizzard(Tent* t,QWidget *parent = nullptr);
    static StartWizzard* execute(Tent*t);
private:
    ConfigWizzardSequence* m_source;
    ConfigEditorSequence* m_editor;

};

#endif // STARTWIZZARD_H
