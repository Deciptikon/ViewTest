#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include "accelerometer.h"
#include "gyroscope.h"
#include "localbasis.h"


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
    void signalCalibrateXAxisAccelerometerIsDone();

public slots:
    void loop();

    // слоты запуска калибровки
    void slotCalibrateZeroPointAccelerometer(const int &msec);
    void slotCalibrateZeroPointGyroscope(const int &msec);

    void slotCalibrateZAxisGyroscope();
    void slotCalibrateXAxisAccelerometer();

private:
    // функции калибровки
    void calibrateZeroPointAccelerometer();
    void calibrateZeroPointGyroscope();

    void calibrateZAxisGyroscope();
    void calibrateXAxisAccelerometer();


    int msecDeltaTime = 100;

    // локальная система координат, определяемая во время калибровок
    LocalBasis localBasis;


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
    float gyrosCoefficient = 1;// коеффициент соответствия показателей

    bool flagCalibrateXAxisAccelerometer = false;
    QVector3D dataCalibrateXAxisAccelerometer;
    int numCalibrateXAxisAccelerometer;
    float accelCoefficient = 1;// коеффициент соответствия показателей

};

#endif // SENSORREADER_H
