#include "sensorreader.h"

#include <QSettings>
#include "constants.h"

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

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    /// чтенние из настроек текущих данных калибровок
    // откалиброванное состояние покоя акселерометра
    float czpax = settings.value(DIR_CALIBRATION
                                 SUBDIR_ACCELEROMETER
                                 KEY_ACCEL_ZERO_POINT_X,
                                 DEFAULT_ACCEL_ZERO_POINT_X).toFloat();
    float czpay = settings.value(DIR_CALIBRATION
                                 SUBDIR_ACCELEROMETER
                                 KEY_ACCEL_ZERO_POINT_Y,
                                 DEFAULT_ACCEL_ZERO_POINT_Y).toFloat();
    float czpaz = settings.value(DIR_CALIBRATION
                                 SUBDIR_ACCELEROMETER
                                 KEY_ACCEL_ZERO_POINT_Z,
                                 DEFAULT_ACCEL_ZERO_POINT_Z).toFloat();

    dataCalibrateZeroPointAccelerometer.setX(czpax);
    dataCalibrateZeroPointAccelerometer.setY(czpay);
    dataCalibrateZeroPointAccelerometer.setZ(czpaz);
    qDebug() << "READING CALIBRATE DATA ACCELEROMETER:" << dataCalibrateZeroPointAccelerometer;


    // откалиброванное состояние покоя гироскопа
    float czpgx = settings.value(DIR_CALIBRATION
                                 SUBDIR_GYROSCOPE
                                 KEY_GYROS_ZERO_POINT_X,
                                 DEFAULT_GYROS_ZERO_POINT_X).toFloat();
    float czpgy = settings.value(DIR_CALIBRATION
                                 SUBDIR_GYROSCOPE
                                 KEY_GYROS_ZERO_POINT_Y,
                                 DEFAULT_GYROS_ZERO_POINT_Y).toFloat();
    float czpgz = settings.value(DIR_CALIBRATION
                                 SUBDIR_GYROSCOPE
                                 KEY_GYROS_ZERO_POINT_Z,
                                 DEFAULT_GYROS_ZERO_POINT_Z).toFloat();

    dataCalibrateZeroPointGyroscope.setX(czpgx);
    dataCalibrateZeroPointGyroscope.setY(czpgy);
    dataCalibrateZeroPointGyroscope.setZ(czpgz);
    qDebug() << "READING CALIBRATE DATA GYROSCOPE:" << dataCalibrateZeroPointGyroscope;

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

        /// записываем эти данные в настройки приложения
        /// dataCalibrateZeroPointAccelerometer --> QSettings
        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

        settings.setValue(DIR_CALIBRATION
                          SUBDIR_ACCELEROMETER
                          KEY_ACCEL_ZERO_POINT_X,
                          dataCalibrateZeroPointAccelerometer.x());
        settings.setValue(DIR_CALIBRATION
                          SUBDIR_ACCELEROMETER
                          KEY_ACCEL_ZERO_POINT_Y,
                          dataCalibrateZeroPointAccelerometer.y());
        settings.setValue(DIR_CALIBRATION
                          SUBDIR_ACCELEROMETER
                          KEY_ACCEL_ZERO_POINT_Z,
                          dataCalibrateZeroPointAccelerometer.z());

        settings.sync(); // синхронизируемся и получаем статус
        if(settings.status() == QSettings::NoError) {
            emit signalCalibrateZeroPointAccelerometerIsDone();
        }
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
        /// записываем эти данные в настройки приложения
        /// dataCalibrateZeroPointGyroscope --> QSettings
        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

        settings.setValue(DIR_CALIBRATION
                          SUBDIR_GYROSCOPE
                          KEY_GYROS_ZERO_POINT_X,
                          dataCalibrateZeroPointGyroscope.x());
        settings.setValue(DIR_CALIBRATION
                          SUBDIR_GYROSCOPE
                          KEY_GYROS_ZERO_POINT_Y,
                          dataCalibrateZeroPointGyroscope.y());
        settings.setValue(DIR_CALIBRATION
                          SUBDIR_GYROSCOPE
                          KEY_GYROS_ZERO_POINT_Z,
                          dataCalibrateZeroPointGyroscope.z());

        settings.sync(); // синхронизируемся и получаем статус
        if(settings.status() == QSettings::NoError) {
            emit signalCalibrateZeroPointGyroscopeIsDone();
        }
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
