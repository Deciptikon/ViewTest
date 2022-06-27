#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

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

    void signalCalibrateZAxisGyroscopeIsDone();

public slots:
    void loop();

    // слоты запуска калибровки
    void slotCalibrateZeroPointAccelerometer(const int &msec);
    void slotCalibrateZeroPointGyroscope(const int &msec);

    void slotCalibrateZAxisGyroscope();

private:
    // функции калибровки
    void calibrateZeroPointAccelerometer();
    void calibrateZeroPointGyroscope();

    void calibrateZAxisGyroscope();


    int msecDeltaTime = 100;

    // переменные и флаги содержащие данные калибровки
    bool flagCalibrateZeroPointAccelerometer = false;
    QVector3D dataCalibrateZeroPointAccelerometer;
    int numCalibrateZeroPointAccelerometer;

    bool flagCalibrateZeroPointGyroscope = false;
    QVector3D dataCalibrateZeroPointGyroscope;
    int numCalibrateZeroPointGyroscope;


    bool flagCalibrateZAxisGyroscope = false;
    QVector3D dataCalibrateZAxisGyroscope;
    int numCalibrateZAxisGyroscope;
    QElapsedTimer elapsedTimer;
    float elapsedTime;
    float to2PiZAxis = 1;

};

#endif // SENSORREADER_H
