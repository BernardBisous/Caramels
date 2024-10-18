#ifndef ARCHIVEWIDGET_H
#define ARCHIVEWIDGET_H

#include "Interface/actionwidget.h"
#include "Interface/scrollarea.h"
#include "Interface/statswidget.h"
#include "Interface/switchcheckbox.h"
#include "Interface/toolbutton.h"
#include "config/archive.h"
#include "qformlayout.h"
#include "qlabel.h"
#include <QWidget>




class ArchiveItem: public ActionWidget
{
    Q_OBJECT
public:
    explicit ArchiveItem(Archive c,bool buttons=true, QWidget *parent = nullptr);
    void setShowButtons(bool s);
    Archive archive();

signals:
    void reuse();
private slots:
    void reuseSlot();
    void hoverSlot(bool s);
    void openSlot();

private:
    QLabel* m_name;
    QLabel* m_desc;
    QLabel* m_pix;
    QLabel* m_date;
    QLabel* m_plants;
    QLabel* m_result;
    QLabel* m_supply;
    QFormLayout* m_form;
    ToolButton*  m_reuseButton;
    ToolButton* m_openButton;
    ToolButton* m_edit;
    StatsWidget* m_stats;
    QWidget* m_buttons;
    Archive m_archive;
};

class ArchiveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArchiveWidget(bool abstract,QWidget *parent = nullptr);
    void load();
    void loadArchive(QString s);
    void usePersonnal(bool s);

    void setAstracted(bool newAstracted);

private slots:
    void reuseSlot();
    void sourceSlot();

signals:
    void reuse(Archive a);

private:
    ScrollArea* m_list;
    SwitchCheckBox* m_source;
    bool m_personnal;
    bool m_astracted;

};

#endif // ARCHIVEWIDGET_H
