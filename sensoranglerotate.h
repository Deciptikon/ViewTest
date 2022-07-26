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

    /// Возвращает текущий угол поворота рулевого колеса
    float getCurrentAngle() const;

    /// Возвращает текущий угол поворота ведущих колёс
    float getAngleWheelsRotate() const;

signals:

public slots:
    /// Обновляет данные с сенсоров (читает из регистров и записывает в
    /// приватные поля класса).
    void readData();

    /// Слот, передает данные на устройство по i2c
    void writeData(const int &data);

private:
    /// Вычисляет угол направления колес на основе угла полученного
    /// с датчика угла поворота рулевого колеса
    void setAngleWheelsRotate(int angle);

    int hexAdress;

    int deviceRegAdress;

    float currentAngle = 0;

    float angleWheelsRotate = 0;
};

#endif // SENSORANGLEROTATE_H
