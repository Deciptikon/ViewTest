#include "sensorreader.h"

#include <QSettings>
#include "constants.h"

#define GRAVITY_MS2                 9.80665

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
    AngleRotate.init(0x15);

    Accelerometer.readZeroData();
    Accelerometer.readCoefficient();

    Gyroscope.readZeroData();
    Gyroscope.readCoefficient();

    // читаем локальный базис
    localBasis.readBasis();
}

int SensorReader::getMsecDeltaTime() const
{
    return msecDeltaTime;
}

void SensorReader::loop()
{
    Accelerometer.updateData();
    Gyroscope.updateData();
    AngleRotate.readData();

    calibrateZeroPointAccelerometer();
    calibrateZeroPointGyroscope();

    calibrateZAxisGyroscope();
    calibrateXAxisAccelerometer();

    // данные акселерометра за вычетом покоя в локальных координатах
    QVector3D accelData = localBasis.toLocalBasis(Accelerometer.getData());

    // данные гироскопа за вычетом покоя в локальных координатах
    QVector3D gyrosData = localBasis.toLocalBasis(Gyroscope.getData());

    //localBasis.debug();

    emit updateDataSens(accelData, gyrosData);
    emit updateCurrentAngle(AngleRotate.getAngleWheelsRotate());
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

    Accelerometer.setZeroData({0,0,0});
    Accelerometer.setCoefficient(1);

    QTimer::singleShot(msec, this, [&](){
        flagCalibrateZeroPointAccelerometer = false;
        if(numCalibrateZeroPointAccelerometer == 0) {
            return ;
        }
        dataCalibrateZeroPointAccelerometer = dataCalibrateZeroPointAccelerometer/numCalibrateZeroPointAccelerometer;

        Accelerometer.setCoefficient(GRAVITY_MS2/dataCalibrateZeroPointAccelerometer.length());
        Accelerometer.saveCoefficient();

        Accelerometer.setZeroData(dataCalibrateZeroPointAccelerometer);
        if(Accelerometer.saveZeroData()) {
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

    Gyroscope.setZeroData({0,0,0});
    Gyroscope.setCoefficient(1);

    QTimer::singleShot(msec, this, [&](){
        flagCalibrateZeroPointGyroscope = false;
        if(numCalibrateZeroPointGyroscope == 0) {
            return ;
        }
        dataCalibrateZeroPointGyroscope = dataCalibrateZeroPointGyroscope/numCalibrateZeroPointGyroscope;
        Gyroscope.setZeroData(dataCalibrateZeroPointGyroscope);

        if(Gyroscope.saveZeroData()) {
            emit signalCalibrateZeroPointGyroscopeIsDone();
        }
    });
}

void SensorReader::slotCalibrateZAxisGyroscope()
{
    flagCalibrateZAxisGyroscope = !flagCalibrateZAxisGyroscope;

    if(flagCalibrateZAxisGyroscope) {
        dataCalibrateZAxisGyroscope = {0, 0, 0};
        numCalibrateZeroPointGyroscope = 0;

        Gyroscope.setCoefficient(1);

        elapsedTimer.start();

    } else {
        elapsedTime = elapsedTimer.elapsed()/1000.0;// 1000 msec = 1 sec
        elapsedTimer.invalidate();

        if(dataCalibrateZAxisGyroscope.length() * elapsedTime != NULL) {
            gyrosCoefficient = 2 * M_PI * numCalibrateZAxisGyroscope /
                    (dataCalibrateZAxisGyroscope.length() * elapsedTime);

            Gyroscope.setCoefficient(gyrosCoefficient);
            Gyroscope.saveCoefficient();
        }

        // локальная ось Z сонаправленна с осью вращения гироскопа
        localBasis.setLocalZ(dataCalibrateZAxisGyroscope);
        if(localBasis.saveBasis()) {
            emit signalCalibrateZAxisGyroscopeIsDone();
        }
        localBasis.debug();
    }
}

void SensorReader::slotCalibrateXAxisAccelerometer()
{
    flagCalibrateXAxisAccelerometer = !flagCalibrateXAxisAccelerometer;

    if(flagCalibrateXAxisAccelerometer) {
        dataCalibrateXAxisAccelerometer = {0, 0, 0};
        numCalibrateXAxisAccelerometer = 0;
    } else {
        // локальная ось X сонаправленна с осью разгона
        localBasis.setLocalX(dataCalibrateXAxisAccelerometer);
        if(localBasis.saveBasis()) {
            emit signalCalibrateXAxisAccelerometerIsDone();
        }
        localBasis.debug();
    }
}

void SensorReader::slotCalibrateWheel()
{
    flagCalibrateWheel = !flagCalibrateWheel;

    if(flagCalibrateWheel) {
        qDebug() << "";
        qDebug() << "flagCalibrateWheel = " << flagCalibrateWheel;
        qDebug() << "";
        // здесь нужно записывать и на каждом шаге сохранять
        // либо массив всех значений либо динамически отыскивать
        // максимальное и минимальное значения угла отклонения
    } else {
        // А здесь нужно найти среднее значение по найденым отклонениям
        // и зафиксировать его в настройках приложения.
        qDebug() << "";
        qDebug() << "flagCalibrateWheel = " << flagCalibrateWheel;
        qDebug() << "";
        emit signalCalibrateWheelIsDone();
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
    dataCalibrateZAxisGyroscope += Gyroscope.getData();
    numCalibrateZAxisGyroscope++;
}

void SensorReader::calibrateXAxisAccelerometer()
{
    if(!flagCalibrateXAxisAccelerometer) {
        return;
    }
    dataCalibrateXAxisAccelerometer += Accelerometer.getData();
    numCalibrateXAxisAccelerometer++;
}
