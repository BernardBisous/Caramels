#include "parameter.h"

#define TIME_FACTOR 3600


Parameter::Parameter(QString name, QString units, int id):
    m_id(id),m_name(name),m_units(units)
{

}

float Parameter::timeMultiplicator()
{
    return TIME_FACTOR;
}


void Parameter::load(QDataStream &in)
{
    in >> m_id >> m_name >> m_units;
    int numValues;
    in >> numValues;
    m_values.resize(numValues);
    for (int i = 0; i < numValues; ++i) {
        in >> m_values[i].hourIndex >> m_values[i].value;
    }
}

void Parameter::save(QDataStream &out)
{
    out << m_id << m_name << m_units;
    int s= m_values.size();
    out <<s;

    for (const TimedValue& value : m_values) {
        out << value.hourIndex << value.value;
    }
}

int Parameter::count()
{
    return m_values.count();
}

int Parameter::maxX()
{
    if(m_values.isEmpty())
        return 0;

    return m_values.last().hourIndex;
}

QString Parameter::csvLine(QString sep)
{
    QString out;
    out+=sep+name()+sep+units()+sep+QString::number(id())+sep+description()+sep;

    int x=maxX();

    for(int i=0;i<x+1;i++)
    {
        bool b=false;
        float v=valueAtTime(i,&b);
        if(b)
        {
            out+=QString::number(v);
        }
        out+=sep;
    }
    return out;

}

float Parameter::maxY()
{

    float out=0;

    for(int i=0;i<m_values.count();i++)
    {
        if( m_values[i].value>=out)
        {
           out=m_values[i].value;
        }
    }
    return out;
}

float Parameter::minY()
{
    float out=0;

    for(int i=0;i<m_values.count();i++)
    {
        if(!i|| m_values[i].value<=out)
        {
           out=m_values[i].value;
        }
    }
    return out;
}

void Parameter::rangeY(float *max, float *min)
{
    *min=0;
    *max=0;
    for(int i=0;i<m_values.count();i++)
    {
        if(!i|| m_values[i].value<=*min)
        {
           *min=m_values[i].value;
        }
        if( m_values[i].value>=*max)
        {
           *max=m_values[i].value;
        }
    }

}

void Parameter::movePoints(QList<int> indexes, float value, bool offset)
{
    if(indexes.isEmpty())
    {
        for(int i=0;i<m_values.count();i++)
        {
            if(offset)
                m_values[i].value+=value;
            else
                m_values[i].value*=value;
        }
    }


    for(int i=0;i<indexes.count();i++)
    {
        if(offset)
            m_values[indexes[i]].value+=value;
        else
            m_values[indexes[i]].value*=value;
    }


}

void Parameter::removePoint(QList<int> indexes)
{
    for(int i=0;i<indexes.count();i++)
       m_values.removeAt(indexes[i]-i);
}

void Parameter::addPoint(int indexHour, float value)
{
    TimedValue v{indexHour,value};
    addPoint(v);
}

void Parameter::addPoint(TimedValue v)
{

    for(int i=0;i<m_values.count();i++)
    {
        if(m_values[i].hourIndex>=v.hourIndex)
            m_values.insert(i,v);
    }
}

void Parameter::addDefaultPoint(int index)
{

    if(m_values.isEmpty())
    {
        addPoint(0,0);
        return;
    }

    TimedValue v;
    if(index<0 || index>=count())
    {
        v=m_values.last();
        addPoint(v);
        return;
    }

    v=m_values[index];
    addPoint(v);




}

QString Parameter::userValueAt(int i)
{
    return QString::number(m_values[i].value)+m_units;
}

TimedValue Parameter::at(int i)
{
    if(i<0 || i>=count())
        return TimedValue{0,0};

    return m_values[i];
}

float Parameter::currentValue(QDateTime startTime,bool*e)
{
    int h=startTime.secsTo(QDateTime::currentDateTime())/TIME_FACTOR;
    return valueAtTime(h,e);
}

float Parameter::valueAtTime(int h, bool *e)
{

    *e=true;
    for(int i=0;i<m_values.count();i++)
    {
        if( m_values[i].hourIndex>=h)
        {
            if(i)
                return m_values[i].value;

            else
                 return m_values[0].value;
        }
    }
    *e=false;
    return 0;
}

int Parameter::closerIndex(int h)
{
    for(int i=0;i<m_values.count();i++)
    {
        if( m_values[i].hourIndex>=h)
        {
            if(i)
                return i;

            else
                 return 0;
        }
    }

    return 0;
}

void Parameter::clear()
{
    m_values.clear();
}

int Parameter::append(TimedValue t)
{


    for(int i=0;i<m_values.count();i++)
    {
        if(i && m_values[i].hourIndex>=t.hourIndex)
        {
            m_values.insert(i,t);


            return i;
        }
    }


    m_values.append(t);
    return m_values.count()-1;
}



void Parameter::setName(const QString &newName)
{
    m_name = newName;
}



void Parameter::setUnits(const QString &newUnits)
{
    m_units = newUnits;
}


void Parameter::setId(int newId)
{
    m_id = newId;
}

QString Parameter::name() const
{
    return m_name;
}

int Parameter::id() const
{
    return m_id;
}

QString Parameter::units() const
{
    return m_units;
}



QString Parameter::description() const
{
    return m_description;
}

void Parameter::setDescription(const QString &newDescription)
{
    m_description = newDescription;
}
