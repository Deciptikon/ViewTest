#include "sensorreader.h"

SensorReader::SensorReader(QObject *parent) : QObject(parent)
{

}

void SensorReader::init(const int msec)
{
    //
}

int SensorReader::getMsecDeltaTime() const
{
    return msecDeltaTime;
}
