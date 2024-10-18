#ifndef ARCHIVEWIZZARD_H
#define ARCHIVEWIZZARD_H

#include "hardware/tent.h"
#include "widgets/archivewidget.h"
#include "wizzard.h"
#include <QWidget>

class ArchiveOverviewSequence: public WizzardSequence
{
    Q_OBJECT
public:
    explicit ArchiveOverviewSequence(QWidget *parent=nullptr);
   virtual void start();

    Archive *archive() const;
    void setArchive(Archive* newArchive);

private:
    Archive* m_archive;
    ArchiveItem* m_preview;
    QWidget* m_previewWidget;
};

class ArchiveWizzard :  public Wizzard
{
    Q_OBJECT
public:
    explicit ArchiveWizzard(Tent* t,QWidget*parent=nullptr);
    virtual void reactToSequenceFinished(WizzardSequence*s,WizzardResult r);
    static ArchiveWizzard *archive(Tent* t, QWidget *parent);
    void archiveExport();
    void archiveExport(QDir d);
    virtual void finish();

signals:
    void archived(Archive);
private:
    WizzardSequence* m_welcome;
    WizzardCapture* m_capture;
    WizzardValue* m_volume;
    WizzardValue* m_result;
    ArchiveOverviewSequence* m_final;

    Archive m_archive;
    Tent* m_tente;

};

#endif // ARCHIVEWIZZARD_H
