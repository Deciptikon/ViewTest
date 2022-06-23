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

    void signalCalibrateZeroPointAccelerometerIsDone();

public slots:
    void loop();


    void slotCalibrateZeroPointAccelerometerl(const int &msec);


private:
    void calibrateZeroPointAccelerometer();

    int msecDeltaTime = 100;

    bool flagCalibrateZeroPointAccelerometer = false;
    QVector3D dataCalibrateZeroPointAccelerometer;
    int numCalibrateZeroPointAccelerometer;
};

#endif // SENSORREADER_H
