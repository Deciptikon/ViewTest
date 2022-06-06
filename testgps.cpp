#include "testgps.h"

TestGPS::TestGPS(QObject *parent) : QObject(parent)
{

}

void TestGPS::read()
{
    qDebug() << "TEST GPS=============================";

    double step = 0.1;

    double amplitude = 10;

    x += step;

    y = amplitude*sin(x/10.0);


    emit updatePositionXY(x, y);
}
