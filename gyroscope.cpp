#include "gyroscope.h"
#include <QSettings>
#include <QDebug>
#include "constants.h"

Gyroscope::Gyroscope(QObject *parent) : QObject(parent)
{
    // Gyroscope ITG3200--------------------------
    #define GYRO 0x68 //  when AD0 is connected to GND ,gyro address is 0x68.
    //#define GYRO 0x69   when AD0 is connected to VCC ,gyro address is 0x69
    #define GYRO_POWER_CTL 0x1B
    #define GYRO_XOUT_H    0x1D
    #define GYRO_XOUT_L    0x1E
    #define GYRO_YOUT_H    0x1F
    #define GYRO_YOUT_L    0x20
    #define GYRO_ZOUT_H    0x21
    #define GYRO_ZOUT_L    0x22

    #define G_SMPLRT_DIV 0x15
    #define G_DLPF_FS 0x16
    #define G_INT_CFG 0x17
    #define G_PWR_MGM 0x3E
    #define G_TO_READ 8 // 2 bytes for each axis x, y, z
    //------------------------------------------------
    deviceGyro = -1;
}

void Gyroscope::init()
{
#ifdef Q_OS_LINUX
    deviceGyro = wiringPiI2CSetup(GYRO);
    qDebug() << "Setup deviceGyroRegAdress =" << deviceGyro;

    if (deviceGyro == -1) {
        qDebug() << "[Gyroscope::init()] deviceGyro == -1";
    } else {
        qDebug() << "I2C Gyroscope communication successfully setup.\n";

        wiringPiI2CWriteReg8(GYRO, G_PWR_MGM, 0x00);
        wiringPiI2CWriteReg8(GYRO, G_SMPLRT_DIV, 0x07);
        wiringPiI2CWriteReg8(GYRO, G_DLPF_FS, 0x1E);
        wiringPiI2CWriteReg8(GYRO, G_INT_CFG, 0x00);
    }
#else
    #ifdef Q_OS_WIN
        qDebug() << "void Gyroscope::init()";
    #endif
#endif
}

void Gyroscope::updateData()
{
#ifdef Q_OS_LINUX
    if (deviceGyro == -1) {
        qDebug() << "[Gyroscope::loop()] deviceGyro == -1";
    } else {
        int dataXg = wiringPiI2CReadReg16(deviceGyro, GYRO_XOUT_L);
        int dataYg = wiringPiI2CReadReg16(deviceGyro, GYRO_YOUT_L);
        int dataZg = wiringPiI2CReadReg16(deviceGyro, GYRO_ZOUT_L);

        float koeff = 1.0/207000.0;// 1.0/215000.0

        data = QVector3D(
                    -(~(int16_t)dataXg + 1) * koeff,
                    -(~(int16_t)dataYg + 1) * koeff,
                    -(~(int16_t)dataZg + 1) * koeff);
        //qDebug() << "Gyroscope:" << data;
    }
#else
    #ifdef Q_OS_WIN
        qDebug() << "void Gyroscope::updateData()";
        //data = {1,1,1};
    #endif
#endif
}

QVector3D Gyroscope::getData() const
{
    return (data - zeroData) * coefficient;
}

void Gyroscope::setZeroData(const QVector3D &vec)
{
    zeroData = vec;
}

const QVector3D &Gyroscope::getZeroData() const
{
    return zeroData;
}

bool Gyroscope::readZeroData()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    // откалиброванное состояние покоя гироскопа
    float x = settings.value(DIR_CALIBRATION
                             SUBDIR_GYROSCOPE
                             KEY_GYROS_ZERO_POINT_X,
                             DEFAULT_GYROS_ZERO_POINT_X).toFloat();
    float y = settings.value(DIR_CALIBRATION
                             SUBDIR_GYROSCOPE
                             KEY_GYROS_ZERO_POINT_Y,
                             DEFAULT_GYROS_ZERO_POINT_Y).toFloat();
    float z = settings.value(DIR_CALIBRATION
                             SUBDIR_GYROSCOPE
                             KEY_GYROS_ZERO_POINT_Z,
                             DEFAULT_GYROS_ZERO_POINT_Z).toFloat();

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        zeroData = {x, y, z};
        qDebug() << "Вектор состояния покоя успешно загружен !";
        return true;
    }

    qDebug() << "Ошибка загрузки вектора состояния покоя !";
    return false;
}

bool Gyroscope::saveZeroData()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    settings.setValue(DIR_CALIBRATION
                      SUBDIR_GYROSCOPE
                      KEY_GYROS_ZERO_POINT_X,
                      zeroData.x());
    settings.setValue(DIR_CALIBRATION
                      SUBDIR_GYROSCOPE
                      KEY_GYROS_ZERO_POINT_Y,
                      zeroData.y());
    settings.setValue(DIR_CALIBRATION
                      SUBDIR_GYROSCOPE
                      KEY_GYROS_ZERO_POINT_Z,
                      zeroData.z());

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        qDebug() << "Данные состояния покоя успешно сохранены !";
        return true;
    }

    qDebug() << "Ошибка сохранения данных состояния покоя !";
    return false;
}

bool Gyroscope::readCoefficient()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    float k = settings.value(DIR_CALIBRATION
                             SUBDIR_GYROSCOPE
                             KEY_GYROS_COEFFICIENT,
                             DEFAULT_GYROS_COEFFICIENT).toFloat();

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        coefficient = k;
        qDebug() << "Коэффициент соответствия успешно загружен !";
        return true;
    }

    qDebug() << "Ошибка загрузки коэффициента соответствия !";
    return false;
}

bool Gyroscope::saveCoefficient()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    settings.setValue(DIR_CALIBRATION
                      SUBDIR_GYROSCOPE
                      KEY_GYROS_COEFFICIENT,
                      coefficient);

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        qDebug() << "Коеффициент пропорциональности успешно сохранен !";
        return true;
    }

    qDebug() << "Ошибка сохранения коеффициента пропорциональности !";
    return false;
}

float Gyroscope::getCoefficient() const
{
    return coefficient;
}

void Gyroscope::setCoefficient(float newCoefficient)
{
    coefficient = newCoefficient;
}
