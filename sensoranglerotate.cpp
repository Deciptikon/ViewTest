#include "sensoranglerotate.h"
#include <QtMath>

SensorAngleRotate::SensorAngleRotate(QObject *parent) : QObject(parent)
{

}

void SensorAngleRotate::init(int hexAdress)
{
#ifdef Q_OS_LINUX

    if(hexAdress>0x08 && hexAdress<0x80) {//0-128
        this->hexAdress = hexAdress;
    } else {
        this->hexAdress = 0;
    }

    this->deviceRegAdress = wiringPiI2CSetup(this->hexAdress);
    qDebug() << "Setup deviceRegAdress N" << this->hexAdress << "= " << this->deviceRegAdress;
#else
    #ifdef Q_OS_WIN
        qDebug() << "void DeviceI2C::init(int hexAdress) =" << hexAdress;
    #endif
#endif
}

void SensorAngleRotate::writeData(const int &data)
{
#ifdef Q_OS_LINUX
    if (this->deviceRegAdress == -1) {
        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
    } else {
        wiringPiI2CWrite(this->deviceRegAdress, data);
        qDebug() << "SlaveController::writeData()";
    }
#else
    #ifdef Q_OS_WIN
        qDebug() << "void DeviceI2C::writeData(const int &data)";
    #endif
#endif
}

void SensorAngleRotate::readData()
{
#ifdef Q_OS_LINUX
    if (this->deviceRegAdress == -1) {
        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
        return;
    }

    uint8_t h = 120; // команда на чтение старшего разряда
    uint8_t l = 121; // команда на чтение младшего разряда

    int receivedDataH = wiringPiI2CReadReg8(this->deviceRegAdress, h);
    int receivedDataL = wiringPiI2CReadReg8(this->deviceRegAdress, l);

    int rd = abs(receivedDataH) * 100 + abs(receivedDataL);

    if( receivedDataH<0 || receivedDataL<0) {
        rd = -rd;
    }

    emit readFrom(rd);

    qDebug() << "-----------------------------------------------";
    qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read 1: " << received_data;
    //qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read 2: " << received_data2;
    qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read summ: " << rd;

#else
    #ifdef Q_OS_WIN
        qDebug() << "void SensorAngleRotate::updateData()";
    #endif
#endif
}

