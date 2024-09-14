#ifndef PLANTATION_H
#define PLANTATION_H

#include "growconfig.h"
#include "qdatetime.h"
#include <QObject>
#include <QTimer>

class Commentary {
    QString body;
    QDateTime time;
};

class Plantation : public QObject
{
    Q_OBJECT
public:
    explicit Plantation(QObject *parent = nullptr);
    bool load(QString filename);
    bool load(QDataStream&);
    void save(QDataStream&);
    void addNote(QString s);
    void start(GrowConfig* c);
    void startConfig(QString name);
    void begin();

    GrowConfig *config() const;

    QDateTime startDate() const;

signals:
    void configSample(int hourIndex);

private slots:
    void timerSlot();

private:
    QDateTime m_startDate;
    QString m_configPath;
    GrowConfig* m_config;
    QTimer* m_timer;
};

#endif // PLANTATION_H
