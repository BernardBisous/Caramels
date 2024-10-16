#ifndef ARCHIVE_H
#define ARCHIVE_H


#include "qdatetime.h"
#include "qpixmap.h"
class Archive
{
public:
    Archive();
    void load(QString path);
    void save(QString path);
    QString name;
    QDate start;
    QDate end;
    int result;
    QPixmap pixmap;
    int plants;
    QString link;
    QString supplier;
    QString description;
    QHash<QString,QString> meta;
};

#endif // ARCHIVE_H
