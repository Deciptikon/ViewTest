#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <QObject>
#include <QDebug>
#include <QVector3D>

#ifdef Q_OS_WIN
    //пока здесь ничего нет
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

class Accelerometer : public QObject
{
    Q_OBJECT
public:
    explicit Accelerometer(QObject *parent = nullptr);

    void init();

    void updateData();

    QVector3D getData() const;

signals:

private:
    QVector3D data;

    int deviceAccel;//идентификатор устройства (акселерометра)
};

#endif // ACCELEROMETER_H
