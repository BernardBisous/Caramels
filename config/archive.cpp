#include "archive.h"
#include "qdir.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include <QFile>


#define CONFIG_NAME "config.csv"

Archive::Archive():result(0),pixmap(),plants(0),meta()
{

}

void Archive::load(QString p)
{

   path=p;
   QFile jsonFile(p+"/archive.json");

   if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

       return;
   }
    QByteArray jsonData = jsonFile.readAll();
   jsonFile.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (jsonDoc.isNull()) {
       return ;
   }


   QJsonObject o = jsonDoc.object();


   name=o["NAME"].toString();
   description=o["DESC"].toString();
   link=o["LINK"].toString();
   supplier=o["SUPPLIER"].toString();
   result=o["RESULT"].toInt();
   plants=o["PLANTS"].toInt();
   start= QDate::fromString(o["START"].toString(),"dd.MM.yyyy");
   end= QDate::fromString(o["END"].toString(),"dd.MM.yyyy");
   pixmap=QPixmap(path+"/"+o["PICTURE"].toString());




   QJsonObject obs=o["STATS"].toObject();
   QStringList lso=obs.keys();
   for(int i=0;i<lso.count();i++)
   {
        stats.insert(lso[i],obs.value(lso[i]).toInt());
   }



}

void Archive::save(QString path)
{
    QString p=name;
    p.remove(" ");

    QDir d(path);


    QJsonObject o;
    o["NAME"]=name;
    o["RESULT"]=result;
    o["PLANTS"]=result;
    o["DESC"]=description;
    o["LINK"]=link;
    o["PICTURE"]="picture.png";
    o["START"]=start.toString("dd.MM.yyy");
    o["END"]=end.toString("dd.MM.yyy");
    o["SUPPLIER"]=supplier;

    QStringList ls=meta.keys();
    for(int i=0;i<ls.count();i++)
    {
        QString k=ls[i];
        o[k]=meta[k];
    }

    QJsonObject obk;
    QStringList lss=stats.keys();
    for(int i=0;i<lss.count();i++)
    {
        QString k=lss[i];
        obk[k]=stats[k];;
    }
    o["STATS"]=obk;




    QFile file(d.absoluteFilePath("archive.json"));

    QJsonDocument jsonDocument(o);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(jsonDocument.toJson());
    file.close();

    pixmap.save(d.absoluteFilePath("picture.png"));



}

bool Archive::hasConfig()
{
    QFile f(configPath());
    return f.exists();
}

void Archive::addStats(QString s, int v)
{
    stats.insert(s,v);
}

QString Archive::configName()
{
    return CONFIG_NAME;

}

void Archive::setDir(QDir d)
{
    path=d.absolutePath();
}

QString Archive::configPath()
{
    QDir d(path);
    if(d.exists())
        return d.absoluteFilePath(configName());
    return "";
}
