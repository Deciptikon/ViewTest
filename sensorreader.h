#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>

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


private:
    int msecDeltaTime = 100;
};

#endif // SENSORREADER_H
