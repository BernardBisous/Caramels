#include "archive.h"
#include "qdir.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include <QFile>

Archive::Archive()
{

}

void Archive::load(QString path)
{

   QFile jsonFile(path+"/archive.json");

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

   QStringList ls=o.keys();
   ls.removeOne("NAME");
   ls.removeOne("PLANTS");
   ls.removeOne("RESULT");
   ls.removeOne("START");
   ls.removeOne("END");
   ls.removeOne("LINK");
   ls.removeOne("DESC");
    ls.removeOne("SUPPLIER");
   ls.removeOne("PICTURE");

   for(int i=0;i<ls.count();i++)
   {
       meta.insert(ls[i],o[ls[i]].toString());
   }

   name=o["NAME"].toString();
   description=o["DESC"].toString();
   link=o["LINK"].toString();
   supplier=o["SUPPLIER"].toString();
   result=o["RESULT"].toInt();
   plants=o["PLANTS"].toInt();
   start= QDate::fromString(o["START"].toString(),"dd.MM.yyyy");
   end= QDate::fromString(o["END"].toString(),"dd.MM.yyyy");
   pixmap=QPixmap(path+"/"+o["PICTURE"].toString());

   qDebug()<<"loaded resss"<<result<<o["RESULT"];

}

void Archive::save(QString path)
{
    QString p=name;
    p.remove(" ");

    QDir d(path);
    QStringList s = d.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);

    int i=1;
    while(s.contains(p))
    {
        p=name;
        p.remove(" ");
        p.append(QString::number(i));
        i++;
    }

    d.mkdir(p);
    d.cd(p);

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



    QFile file(d.absoluteFilePath("archive.json"));
    QJsonDocument jsonDocument(o);
    file.write(jsonDocument.toJson());
    file.close();

    pixmap.save("picture.png");

}
