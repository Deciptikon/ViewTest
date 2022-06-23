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

    /// чтенние из настроек текущих данных калибровок
    /// QSettings --> dataCalibrateZeroPointAccelerometer
    /// QSettings --> dataCalibrateZeroPointGyroscope
}

int SensorReader::getMsecDeltaTime() const
{
    return msecDeltaTime;
}

void SensorReader::loop()
{
    Accelerometer.updateData();
    Gyroscope.updateData();

    calibrateZeroPointAccelerometer();
    calibrateZeroPointGyroscope();

    QVector3D accelData = Accelerometer.getData() - dataCalibrateZeroPointAccelerometer;
    QVector3D gyrosData = Gyroscope.getData() - dataCalibrateZeroPointGyroscope;

    emit updateDataSens(accelData, gyrosData);
}

void SensorReader::slotCalibrateZeroPointAccelerometer(const int &msec)
{
    if(flagCalibrateZeroPointAccelerometer) {
        return;
    }
    if(msec <= 0 || msec > 60000) {
        return;
    }

    flagCalibrateZeroPointAccelerometer = true;
    dataCalibrateZeroPointAccelerometer = {0, 0, 0};
    numCalibrateZeroPointAccelerometer  = 0;

    QTimer::singleShot(msec, this, [&](){
        qDebug() << "======================= Calibrate Zero Point Accelerometer ========================";
        flagCalibrateZeroPointAccelerometer = false;
        if(numCalibrateZeroPointAccelerometer == 0) {
            return ;
        }
        dataCalibrateZeroPointAccelerometer = dataCalibrateZeroPointAccelerometer/numCalibrateZeroPointAccelerometer;
        /// записать эти данные в настройки приложения
        /// dataCalibrateZeroPointAccelerometer --> QSettings

        emit signalCalibrateZeroPointAccelerometerIsDone();
    });
}

void SensorReader::slotCalibrateZeroPointGyroscope(const int &msec)
{
    if(flagCalibrateZeroPointAccelerometer) {
        return;
    }
    if(msec <= 0 || msec > 60000) {
        return;
    }

    flagCalibrateZeroPointGyroscope = true;
    dataCalibrateZeroPointGyroscope = {0, 0, 0};
    numCalibrateZeroPointGyroscope  = 0;

    QTimer::singleShot(msec, this, [&](){
        qDebug() << "======================= Calibrate Zero Point Gyroscope ========================";
        flagCalibrateZeroPointGyroscope = false;
        if(numCalibrateZeroPointGyroscope == 0) {
            return ;
        }
        dataCalibrateZeroPointGyroscope = dataCalibrateZeroPointGyroscope/numCalibrateZeroPointGyroscope;
        /// записать эти данные в настройки приложения
        /// dataCalibrateZeroPointGyroscope --> QSettings

        emit signalCalibrateZeroPointGyroscopeIsDone();
    });
}

void SensorReader::calibrateZeroPointAccelerometer()
{
    if(!flagCalibrateZeroPointAccelerometer) {
        return;
    }
    dataCalibrateZeroPointAccelerometer += Accelerometer.getData();
    numCalibrateZeroPointAccelerometer++;
}

void SensorReader::calibrateZeroPointGyroscope()
{
    if(!flagCalibrateZeroPointGyroscope) {
        return;
    }
    dataCalibrateZeroPointGyroscope += Gyroscope.getData();
    numCalibrateZeroPointGyroscope++;
}
