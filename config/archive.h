#ifndef ARCHIVE_H
#define ARCHIVE_H


#include "qdatetime.h"
#include "qdir.h"
#include "qpixmap.h"
class Archive
{
public:
    Archive();
    void load(QString path);
    void save(QString path);
    bool hasConfig();
    void addStats(QString s,int v);
    static QString configName();
    void setDir(QDir d);
    QString name;
    QDate start;
    QDate end;
    int result;
    QPixmap pixmap;
    int plants;
    QString link;
    QString supplier;
    QString description;
    QString path;

    QString configPath();
    QHash<QString,QString> meta;
    QHash<QString,int> stats;
};

#endif // ARCHIVE_H
