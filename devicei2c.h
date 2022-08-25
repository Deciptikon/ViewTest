#ifndef DEVICEI2C_H
#define DEVICEI2C_H

#include <QObject>
#include <QDebug>

#ifdef Q_OS_WIN
    //пока здесь ничего нет
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

const int COMM_STOP = 222;// команда остановки двигателя (сбрасывает пульсации до нуля)

class DeviceI2C : public QObject
{
    Q_OBJECT
public:
    explicit DeviceI2C(QObject *parent = nullptr);

    /// Регистрирует устройство в системе и настраивает его на обмен данными.
    void init(int hexAdress);

signals:
    /// Сигнал, возвращает прочтенные данные при помощи слота readData()
    void readFrom(int &data);

public slots:
    /// Читает данные !!!не реализован!!!
    void readData();

    /// Слот, передает данные [0 - 255] на устройство
    void writeData(const int &data);

    /// Слот, передает большие данные на устройство
    void writeBigData(const int &data);

    void stopDrive();
    void startDrive();

    void slotGetCurrentAngle(const float& angle);

private:
    int hexAdress;

    int deviceRegAdress;

    bool isStopped = false;

    float currentAngle = 0;

};

#endif // DEVICEI2C_H
