#include "growingtent.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GrowingTent w;
    w.show();
    return a.exec();
}
