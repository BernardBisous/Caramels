#include "growconfig.h"
#include <QDataStream>
#include <QFileDialog>

#define CONFIG_PATH "config.weed"
#define COMFIG_CSV_PATH "://config"
#define SUFIX_BINARY ".weed"
#define SUFIX_CSV ".csv"
#define SUFFIX_TXT ".txt"
#define CSV_SEPARATOR ","

GrowConfig::GrowConfig():m_parameters()
{
    if(!openDefault())
        defaultSetup();
}

void GrowConfig::save(QDataStream &c)
{
    c << m_parameters.size();
        // Serialize each parameter
        for (int i=0;i<m_parameters.count();i++) {
            m_parameters[i]->save(c);
        }
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

int GrowConfig::getSize() const
{
    return size;
}

void GrowConfig::setSize(int newSize)
{
    size = newSize;
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



bool GrowConfig::load(QDataStream& c) {

    if(c.atEnd())
        return false;

    int numParameters;
    c >> numParameters;

    m_parameters.clear();
    for (int i = 0; i < numParameters; ++i) {
        Parameter* parameter=new Parameter;
        parameter->load(c);
        m_parameters.append(parameter);
    }

    return true;
}

int GrowConfig::maxHours()
{
    int out=0;

    for(int i=0;i<m_parameters.count();i++)
    {
        int m=m_parameters[i]->maxX();
        if(out<m)
            out=m;
    }
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


bool GrowConfig::openDefault() {



    if(open(CONFIG_PATH))
        return true;

    return loadCsv(COMFIG_CSV_PATH);




}
bool GrowConfig::loadCsv(QString filename) {

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


       // Check if the header has the expected format


       // Read the data rows
       while (!file.atEnd()) {
           QString dataLine = file.readLine();
           line = dataLine.split(CSV_SEPARATOR);

           QString name = line[Name];
           QString units = line[Units];
           int id = line[Id].toInt();
           QString desc=line[Description];
           QString group=line[Group];

           // Create a new Parameter object
           Parameter* parameter = fromName(name);
           if(!parameter)
                parameter=addParameter(name, units, id);
           else parameter->clear();

           parameter->setDescription(desc);


           for(int i=Values;i<line.count();i++)
           {
               QString s=line[i];
               int hour=header[i].toInt();
               if(!s.isEmpty())
               {
                   bool ok;
                   float value = line[i].toFloat(&ok);
                   if(ok)
                   {
                       TimedValue v;
                       v.hourIndex=hour;
                       v.value=value;
                       parameter->append(v);
                       //qDebug()<<"loaded value"<<value<<hour<<name;
                   }
               }

           }

          // qDebug()<<"loaded param"<<name<<parameter->count()<<parameter->maxX()<<parameter->maxY();



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
        // Gérer l'erreur si le fichier ne peut pas être ouvert
       // qDebug()<<"cannot open config file "<<filename;

        return false;
    }

    QDataStream in(&file);
    bool c= load(in);
    file.close();
    return c;
}



QList<TimedValue> Parameter::values() const
{
    return m_values;
}
