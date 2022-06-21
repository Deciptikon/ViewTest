#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <QObject>
#include <QDebug>
#include <QVector3D>
#include <QtMath>

#ifdef Q_OS_WIN
    //пока здесь ничего нет
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

class Gyroscope : public QObject
{
    Q_OBJECT
public:
    explicit Gyroscope(QObject *parent = nullptr);

    void init();

    void updateData();

    QVector3D getData() const;

signals:

private:
    QVector3D data;

    int deviceGyro;//идентификатор устройства (гироскопа)
};

#endif // GYROSCOPE_H
