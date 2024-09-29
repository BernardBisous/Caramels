#include "growingtent.h"
#include <QApplication>

#define RELEASE false
#define FILE_PATH "logs.txt"

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    default: txt = QString("Fatal: %1").arg(msg);

    };

    QFile outFile(FILE_PATH);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << Qt::endl;
    outFile.close();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GrowingTent w;

    if(RELEASE)
    {

        QFile outFile(FILE_PATH);
        outFile.resize(0);
        qInstallMessageHandler(myMessageHandler);
    }

    w.showFullScreen();
    w.setFixedSize(w.size());
    return a.exec();
}
