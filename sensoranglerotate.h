#ifndef SENSORANGLEROTATE_H
#define SENSORANGLEROTATE_H

#include <QObject>
#include <QDebug>

#ifdef Q_OS_WIN
    //пока здесь ничего нет
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

class SensorAngleRotate : public QObject
{
    Q_OBJECT
public:
    explicit SensorAngleRotate(QObject *parent = nullptr);

    /// Регистрирует устройство в системе и настраивает его на обмен данными.
    void init(int hexAdress);

signals:
    //

public slots:
    /// Обновляет данные с сенсоров (читает из регистров и записывает в
    /// приватные поля класса).
    void readData();

    /// Слот, передает данные на устройство
    void writeData(const int &data);

private:
    int hexAdress;

    int deviceRegAdress;

    int counter;
    float currentAngle;
};

#endif // SENSORANGLEROTATE_H
