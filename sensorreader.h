#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>
#include <QTimer>

#include "accelerometer.h"
#include "gyroscope.h"


class SensorReader : public QObject
{
    Q_OBJECT
public:
    explicit SensorReader(QObject *parent = nullptr);

    void init(const int msec=100);

    int getMsecDeltaTime() const;

    Accelerometer Accelerometer;
    Gyroscope     Gyroscope;

signals:
    void updateDataSens(const QVector3D &accel, const QVector3D &gyro);

    // сигнал о успешной калибровке для QML
    void signalCalibrateZeroPointAccelerometerIsDone();
    void signalCalibrateZeroPointGyroscopeIsDone();

public slots:
    void loop();

    // слоты запуска калибровки
    void slotCalibrateZeroPointAccelerometer(const int &msec);
    void slotCalibrateZeroPointGyroscope(const int &msec);

private:
    // функции калибровки
    void calibrateZeroPointAccelerometer();
    void calibrateZeroPointGyroscope();

    int msecDeltaTime = 100;

    // переменные и флаги содержащие данные калибровки
    bool flagCalibrateZeroPointAccelerometer = false;
    QVector3D dataCalibrateZeroPointAccelerometer;
    int numCalibrateZeroPointAccelerometer;

    bool flagCalibrateZeroPointGyroscope = false;
    QVector3D dataCalibrateZeroPointGyroscope;
    int numCalibrateZeroPointGyroscope;
};

#endif // SENSORREADER_H