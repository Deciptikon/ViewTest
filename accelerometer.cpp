#include "accelerometer.h"
#include <QSettings>
#include <QDebug>
#include "constants.h"

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
        //data = {10,10,10};
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
    return (data - zeroData) * coefficient;
}

void Accelerometer::setZeroData(const QVector3D &vec)
{
    zeroData = vec;
}

const QVector3D &Accelerometer::getZeroData() const
{
    return zeroData;
}

bool Accelerometer::readZeroData()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    // откалиброванное состояние покоя акселерометра
    float x = settings.value(DIR_CALIBRATION
                             SUBDIR_ACCELEROMETER
                             KEY_ACCEL_ZERO_POINT_X,
                             DEFAULT_ACCEL_ZERO_POINT_X).toFloat();
    float y = settings.value(DIR_CALIBRATION
                             SUBDIR_ACCELEROMETER
                             KEY_ACCEL_ZERO_POINT_Y,
                             DEFAULT_ACCEL_ZERO_POINT_Y).toFloat();
    float z = settings.value(DIR_CALIBRATION
                             SUBDIR_ACCELEROMETER
                             KEY_ACCEL_ZERO_POINT_Z,
                             DEFAULT_ACCEL_ZERO_POINT_Z).toFloat();

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        zeroData = {x, y, z};
        qDebug() << "Вектор состояния покоя успешно загружен !";
        return true;
    }

    qDebug() << "Ошибка загрузки вектора состояния покоя !";
    return false;
}


bool Accelerometer::saveZeroData()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    settings.setValue(DIR_CALIBRATION
                      SUBDIR_ACCELEROMETER
                      KEY_ACCEL_ZERO_POINT_X,
                      zeroData.x());
    settings.setValue(DIR_CALIBRATION
                      SUBDIR_ACCELEROMETER
                      KEY_ACCEL_ZERO_POINT_Y,
                      zeroData.y());
    settings.setValue(DIR_CALIBRATION
                      SUBDIR_ACCELEROMETER
                      KEY_ACCEL_ZERO_POINT_Z,
                      zeroData.z());

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        qDebug() << "Данные состояния покоя успешно сохранены !";
        return true;
    }

    qDebug() << "Ошибка сохранения данных состояния покоя !";
    return false;
}

bool Accelerometer::readCoefficient()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    float k = settings.value(DIR_CALIBRATION
                             SUBDIR_ACCELEROMETER
                             KEY_ACCEL_COEFFICIENT,
                             DEFAULT_ACCEL_COEFFICIENT).toFloat();

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        coefficient = k;
        qDebug() << "Коэффициент соответствия успешно загружен !";
        return true;
    }

    qDebug() << "Ошибка загрузки коэффициента соответствия !";
    return false;
}

bool Accelerometer::saveCoefficient()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    settings.setValue(DIR_CALIBRATION
                      SUBDIR_ACCELEROMETER
                      KEY_ACCEL_COEFFICIENT,
                      coefficient);

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        qDebug() << "Коеффициент пропорциональности успешно сохранен !";
        return true;
    }

    qDebug() << "Ошибка сохранения коеффициента пропорциональности !";
    return false;
}

float Accelerometer::getCoefficient() const
{
    return coefficient;
}

void Accelerometer::setCoefficient(float newCoefficient)
{
    coefficient = newCoefficient;
}


