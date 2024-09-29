#include "growconfig.h"
#include "hardware/Pinout.h"
#include <QDataStream>
#include <QFileDialog>

#define CONFIG_PATH "config2.weed"
#define COMFIG_CSV_PATH "://config"
#define COMFIG_CSV_FILE_PATH "Config.csv"
#define SUFIX_BINARY ".weed"
#define SUFIX_CSV ".csv"
#define SUFFIX_TXT ".txt"
#define CSV_SEPARATOR ","

GrowConfig::GrowConfig():m_parameters(),m_name("Grape Gazz 🥵")
{
    m_events=new Events;

    if(!openDefault())
        loadCsv(COMFIG_CSV_PATH);

    computeMaxHours();
}

bool GrowConfig::save()
{
    return save(CONFIG_PATH);
}

bool GrowConfig::save(QString s)
{
    QFile file(s);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {

        return false;
    }
    QDataStream out(&file);
    save(out);
    return true;
}

void GrowConfig::save(QDataStream &c)
{


    c <<name();
    int t=m_parameters.size();
    c << t;
    // Serialize each parameter

    for (int i=0;i<m_parameters.count();i++) {
        m_parameters[i]->save(c);
    }

    m_events->save(c);
}

int GrowConfig::countParameters()
{
    return m_parameters.count();
}



GrowConfig GrowConfig::open(bool *ok)
{
    GrowConfig config;
    if (config.openDefault()) {
        *ok = true;
    } else {
        *ok = false;
    }
    return config;
}



QStringList GrowConfig::parameterNames()
{
    QStringList  out;
    for(int i=0;i<m_parameters.count();i++)
        out<<m_parameters[i]->name();

    return out;
}

Parameter *GrowConfig::parameterAddr(int i)
{
    return m_parameters[i];
}

Parameter *GrowConfig::createDefault()
{
    return addParameter();
}

void GrowConfig::defaultSetup()
{
    addParameter("Lights","%");
    addParameter("Water/L","%");

}



Parameter *GrowConfig::addParameter(QString s, QString units,int id)
{
    Parameter*p=new Parameter(s,units, id);
    m_parameters.append(p);
    return p;
}

Parameter *GrowConfig::fromName(QString s)
{
    for(int i=0;i<m_parameters.size();i++)
        if(m_parameters[i]->name()==s)
            return m_parameters[i];

    return nullptr;
}

QString GrowConfig::name() const
{
    return m_name;
}

void GrowConfig::setName(const QString &newName)
{
    m_name = newName;
}

int GrowConfig::maxHours() const
{
    return m_maXHours;
}

Events *GrowConfig::events() const
{
    return m_events;
}

void GrowConfig::saveCsv(QString file)
{
    QString sep=",";
    QString line="\n";

    QFile f(file);
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream s(&f);
    s.setEncoding(QStringConverter::Utf8);
    QString h;
    h+=name()+sep;

    s<<h;
    for(int i=0;i<m_parameters.count();i++)
    {
        s<<line+m_parameters[i]->csvLine(sep);
    }
    f.close();
}




bool GrowConfig::load(QDataStream& c) {

    if(c.atEnd())
        return false;

    clear();


    c>>m_name;
    int numParameters;
    c >> numParameters;

    for (int i = 0; i < numParameters; ++i) {
        Parameter* parameter=new Parameter;
        parameter->load(c);
        m_parameters.append(parameter);
    }

    m_events->load(c);
 //   qDebug()<<"loaded config"<<m_parameters.count()<<m_events->count();
    return true;
}

int GrowConfig::computeMaxHours()
{
    int out=0;

    for(int i=0;i<m_parameters.count();i++)
    {
        int m=m_parameters[i]->maxX();
        if(out<m)
            out=m;
    }
    m_maXHours=out;
    return out;
}

float GrowConfig::maxY()
{
    float out=0;

    for(int i=0;i<m_parameters.count();i++)
    {
        float m=m_parameters[i]->maxY();
        if(out<m)
            out=m;
    }
    return out;
}

void GrowConfig::clear()
{
    for(int i=0;i<m_parameters.count();i++)
        delete m_parameters[i];

    m_parameters.clear();

    m_events->clear();
}

Parameter *GrowConfig::loadParameterCSVLine(QString dataLine, QStringList header)
{





    QStringList line;
    line = dataLine.split(CSV_SEPARATOR);

    QString name = line[Name];
    QString units = line[Units];
    int id = line[Id].toInt();
    QString desc=line[Description];
    QString group=line[Group];

     Parameter* parameter=nullptr;

     bool event=id>=EVENTS;
    if(event)
    {
        m_events->clear();
    }


    else
    {
      parameter = fromName(name);
      if(!parameter)
           parameter=addParameter(name, units, id);

      else parameter->clear();

      parameter->setDescription(desc);
    }




    for(int i=Values;i<line.count();i++)
    {
        QString s=line[i];
        int hour=header[i].toInt();

        if(!s.isEmpty())
        {

            if(event)
            {
                m_events->add(s,hour);
            }
            else
            {
                bool ok;

                float value = line[i].toFloat(&ok);

                if(ok)
                {

                    TimedValue v;
                    v.hourIndex=hour;
                    v.value=value;
                    parameter->append(v);

                }
            }

        }

    }

    return parameter;
}




bool GrowConfig::openDefault()
{



    if(!loadCsv(COMFIG_CSV_FILE_PATH))
    {
        qDebug()<<"dfgergwssss";

        if(QFile::exists(COMFIG_CSV_FILE_PATH))
            QFile::remove(COMFIG_CSV_FILE_PATH);

        QFile fe(COMFIG_CSV_PATH);
        fe.open(QIODevice::ReadOnly);

        QFile ft(COMFIG_CSV_FILE_PATH);
        ft.open(QIODevice::WriteOnly | QIODevice::Truncate);

        ft.write(fe.readAll());
        ft.close();
        fe.close();

        qDebug()<<"dfgergwer";
    }

    return loadCsv(COMFIG_CSV_FILE_PATH);

}
bool GrowConfig::loadCsv(QString filename) {

    clear();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open CSV file:" << filename;
        return false;
    }

    QStringList header;
       QStringList line;

       // Read the header row
       QString headerLine = file.readLine();
       header = headerLine.split(CSV_SEPARATOR);


       if(header.isEmpty())
           return false;

       setName(header.first());
       // Check if the header has the expected format


       // Read the data rows
       while (!file.atEnd()) {
           QString dataLine = file.readLine();
           loadParameterCSVLine(dataLine,header);
       }

       file.close();
       return true;
}

int GrowConfig::indexOf(Parameter *c)
{
    return m_parameters.indexOf(c);
}

bool GrowConfig::browse()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,"Ouvrir un fichier de configuration","","Fichiers de configuration (*.csv)");
    if (fileName.isEmpty()) {
        return false; // L'utilisateur a annulé
    }

    QFileInfo info(fileName);
    if(!info.exists())
        return false;

    if(info.suffix()==SUFIX_BINARY)
        return open(fileName);

    return loadCsv(fileName);
}

bool GrowConfig::open(QString filename)
{

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {

        return false;
    }

    m_name=filename;
    QDataStream in(&file);
    bool c= load(in);
    file.close();
    return c;
}



QList<TimedValue> Parameter::values() const
{
    return m_values;
}
