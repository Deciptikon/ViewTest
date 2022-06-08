#ifndef DEVICEI2C_H
#define DEVICEI2C_H

#include <QObject>
#include <QDebug>

#include <wiringPi.h>
#include <wiringPiI2C.h>

class DeviceI2C : public QObject
{
    Q_OBJECT
public:
    explicit DeviceI2C(QObject *parent = nullptr);

    void init(int hexAdress);

signals:
    void readFrom(int &data);

public slots:
    void readData();
    void writeData(const int &data);

private:
    int hexAdress;

    int deviceRegAdress;

};

#endif // DEVICEI2C_H
