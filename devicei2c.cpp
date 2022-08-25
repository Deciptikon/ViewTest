#include "devicei2c.h"

#include "sensorreader.h"

DeviceI2C::DeviceI2C(QObject *parent) : QObject(parent)
{

}

void DeviceI2C::init(int hexAdress)
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
        Q_UNUSED(hexAdress);
        qDebug() << "void DeviceI2C::init(int hexAdress)";
    #endif
#endif
}

void DeviceI2C::readData()
{
    //    if (this->deviceRegAdress == -1) {
    //        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
    //    } else {
    //        uint8_t g = 'g';
    //        uint8_t f = 'f';

    //        int received_data  = wiringPiI2CReadReg8(this->deviceRegAdress, g);
    //        int received_data2 = wiringPiI2CReadReg8(this->deviceRegAdress, f);

    //        int rd = received_data2*100 + received_data;
    //        emit readFrom(rd);

    //        qDebug() << "-----------------------------------------------";
    //        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read 1: " << received_data;
    //        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read 2: " << received_data2;
    //        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read summ: " << rd;
    //    }
}

void DeviceI2C::writeData(const int &data)
{
#ifdef Q_OS_LINUX
    if (this->deviceRegAdress == -1) {
        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
        return;
    }

    wiringPiI2CWrite(this->deviceRegAdress, data);
    qDebug() << "SlaveController::writeData()";

#else
    #ifdef Q_OS_WIN
        Q_UNUSED(data);
        qDebug() << "void DeviceI2C::writeData(const int &data)";
    #endif
#endif
}

void DeviceI2C::writeBigData(const int &data)
{
    if(isStopped) {
        if((data>0 && currentAngle>0) || (data<0 && currentAngle<0)) {
            return;
        }
    }
#ifdef Q_OS_LINUX
    if (this->deviceRegAdress == -1) {
        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
        return;
    }
    if(data == 0) {
        return;
    }

    const int amplituda = 100;
    int k = 1;
    if(data < 0) {
        k = -1;
    }

    int16_t tData = data;

    while(tData != 0) {
        int pocketData = 0;
        if(abs(tData) <= amplituda) {
            pocketData = tData;
            tData = 0;
        } else {
            pocketData = k * amplituda;
            tData -= pocketData;
        }
        uint8_t comm = amplituda + pocketData;
        wiringPiI2CWrite(this->deviceRegAdress, comm);
        qDebug() << "SlaveController::writeData(): " << pocketData;
    }
#else
    #ifdef Q_OS_WIN
        qDebug() << "void DeviceI2C::writeData(const int &data)";
    #endif
#endif
}

void DeviceI2C::stopDrive()
{
    writeData(COMM_STOP);
    isStopped = true;

//    Это будет работать для объектов из разных потоков ???
//    SensorReader* sr = qobject_cast<SensorReader*>(sender());
    //    currentAngle = sr->AngleRotate.getAngleWheelsRotate();
}

void DeviceI2C::startDrive()
{
    isStopped = false;
}

void DeviceI2C::slotGetCurrentAngle(const float &angle)
{
    currentAngle = angle;
}
