#include "parameter.h"


Parameter::Parameter(QString name, QString units, int id):
    m_id(id),m_name(name),m_units(units),m_devices()
{

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
    out << m_values.size();
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

void Parameter::attach(Device *d)
{
    if(m_devices.contains(d))
        return;

    m_devices.append(d);
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

QString Parameter::group() const
{
    return m_group;
}

void Parameter::setGroup(const QString &newGroup)
{
    m_group = newGroup;
}

QList<Device *> Parameter::devices() const
{
    return m_devices;
}
