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

public slots:
    void loop();


    void slotCalibrateAccelerometerNull(const int &msec);


private:
    void calibrateAccelerometerNull();

    int msecDeltaTime = 100;

    bool flagCalibrateAccelerometerNull = false;
    QVector3D dataCalibrateAccelerometerNull;
    int numCalibrateAccelerometerNull;
};

#endif // SENSORREADER_H
