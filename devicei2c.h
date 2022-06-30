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

    /// Слот, передает данные на устройство
    void writeData(const int &data);

private:
    int hexAdress;

    int deviceRegAdress;

};

#endif // DEVICEI2C_H
