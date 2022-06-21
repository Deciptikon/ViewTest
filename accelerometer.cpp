#include "accelerometer.h"

Accelerometer::Accelerometer(QObject *parent) : QObject(parent)
{
    // Accelerometer ADXL-345---------------------
    #define ACCELEROMETER 0x53
    #define REG_POWER_CTL   0x2D
    #define REG_DATA_X_LOW  0x32
    #define REG_DATA_X_HIGH 0x33
    #define REG_DATA_Y_LOW  0x34
    #define REG_DATA_Y_HIGH 0x35
    #define REG_DATA_Z_LOW  0x36
    #define REG_DATA_Z_HIGH 0x37
    //--------------------------------------------
    deviceAccel = -1;
}

void Accelerometer::init()
{
#ifdef Q_OS_LINUX
    deviceAccel = wiringPiI2CSetup(ACCELEROMETER);
    qDebug() << "Setup deviceAccelRegAdress =" << deviceAccel;

    if (deviceAccel == -1) {
        qDebug() << "[Accelerometer::init()] deviceAccel == -1";
    } else {
        qDebug() << "I2C Accelerometer communication successfully setup.\n";
        wiringPiI2CWriteReg8(deviceAccel, REG_POWER_CTL, 0b00001000);
    }
#else
    #ifdef Q_OS_WIN
        qDebug() << "void Accelerometer::init()";
    #endif
#endif
}

void Accelerometer::updateData()
{
#ifdef Q_OS_LINUX
    if (deviceAccel == -1) {
        qDebug() << "[Accelerometer::loop()] deviceAccel == -1";
    } else {
        int dataXa = wiringPiI2CReadReg16(deviceAccel, REG_DATA_X_LOW);
        int dataYa = wiringPiI2CReadReg16(deviceAccel, REG_DATA_Y_LOW);
        int dataZa = wiringPiI2CReadReg16(deviceAccel, REG_DATA_Z_LOW);

        data = QVector3D(
                    -(~(int16_t)dataXa + 1),
                    -(~(int16_t)dataYa + 1),
                    -(~(int16_t)dataZa + 1));
        //qDebug() << "Accelerometer: " << data;
    }
#else
    #ifdef Q_OS_WIN
        qDebug() << "void Accelerometer::updateData()";
    #endif
#endif
}

QVector3D Accelerometer::getData() const
{
    return data;
}
