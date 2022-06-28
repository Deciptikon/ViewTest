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


    dataCalibrateZAxisGyroscope = {1,1,1};

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

    calibrateZAxisGyroscope();


    QVector3D zeroDataAccel = Accelerometer.getData() - dataCalibrateZeroPointAccelerometer;
    QVector3D accelData = zeroDataAccel;


    QVector3D zeroDataGyros = (Gyroscope.getData() - dataCalibrateZeroPointGyroscope) * to2PiZAxis;
    float zgd = QVector3D::dotProduct( dataCalibrateZAxisGyroscope.normalized(), zeroDataGyros);
    float ygd = 0;
    float xgd = sqrtf(zeroDataGyros.lengthSquared() - powf(zgd, 2));
    QVector3D gyrosData = {xgd, ygd, zgd};

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

void SensorReader::slotCalibrateZAxisGyroscope()
{
    flagCalibrateZAxisGyroscope = !flagCalibrateZAxisGyroscope;

    if(flagCalibrateZAxisGyroscope) {
        // calibrate
        dataCalibrateZAxisGyroscope = {0, 0, 0};
        numCalibrateZeroPointGyroscope = 0;

        elapsedTimer.start();
    } else {
        // stoppped calibrate

        elapsedTime = elapsedTimer.elapsed()/1000.0;// 1000 msec = 1 sec
        elapsedTimer.invalidate();

        qDebug() << "ELAPSED TIME =" << elapsedTime;

        if(dataCalibrateZAxisGyroscope.length()*elapsedTime) {
            to2PiZAxis = 2 * M_PI / (dataCalibrateZAxisGyroscope.length()*elapsedTime);
        }

        QVector3D zaxis = dataCalibrateZAxisGyroscope*elapsedTime*to2PiZAxis;

        qDebug() << "ZAXIS.length() =" << zaxis.length();
        //zaxis.normalize();

        emit signalCalibrateZAxisGyroscopeIsDone();
    }
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

void SensorReader::calibrateZAxisGyroscope()
{
    if(!flagCalibrateZAxisGyroscope) {
        return;
    }
    dataCalibrateZAxisGyroscope += Gyroscope.getData() - dataCalibrateZeroPointGyroscope;
    qDebug() << "dataCalibrateZAxisGyroscope     =" << dataCalibrateZAxisGyroscope.normalized();

    numCalibrateZAxisGyroscope++;
}
