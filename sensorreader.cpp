#include "sensorreader.h"

SensorReader::SensorReader(QObject *parent) : QObject(parent)
{

}

void SensorReader::init(const int msec)
{
    if(msec > 1) {
        msecDeltaTime = msec;
    } else {
        msecDeltaTime = 1;
    }

    Accelerometer.init();
    Gyroscope.init();
}

int SensorReader::getMsecDeltaTime() const
{
    return msecDeltaTime;
}

void SensorReader::loop()
{
    Accelerometer.updateData();
    Gyroscope.updateData();

    calibrateAccelerometerNull();

    QVector3D accelData = Accelerometer.getData();
    QVector3D gyrosData = Gyroscope.getData();

    emit updateDataSens(accelData, gyrosData);
}

void SensorReader::slotCalibrateAccelerometerNull(const int &msec)
{
    if(flagCalibrateAccelerometerNull) {
        return;
    }
    if(msec <= 0 || msec > 60000) {
        return;
    }

    flagCalibrateAccelerometerNull = true;
    dataCalibrateAccelerometerNull = {0, 0, 0};
    numCalibrateAccelerometerNull  = 0;

    QTimer::singleShot(msec, this, [&](){
        qDebug() << "=======================QTimer::singleShot========================";
        flagCalibrateAccelerometerNull = false;
        ///to do -->
    });
}

void SensorReader::calibrateAccelerometerNull()
{
    if(!flagCalibrateAccelerometerNull) {
        return;
    }
    dataCalibrateAccelerometerNull += Accelerometer.getData();
    numCalibrateAccelerometerNull++;
}
