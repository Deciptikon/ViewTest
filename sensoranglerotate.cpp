#include "sensoranglerotate.h"

#include "constants.h"

#include <QtMath>
#include <QSettings>

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

    const uint8_t COMM_RESET = 11;
    wiringPiI2CReadReg8(this->deviceRegAdress, COMM_RESET);
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
        Q_UNUSED(data);
        qDebug() << "void DeviceI2C::writeData(const int &data)";
    #endif
#endif
}

void SensorAngleRotate::setAngleWheelsRotate(int angle)
{
    angleWheelsRotate = ((float)angle - delta) * koeff;
}

float SensorAngleRotate::getAmplituda() const
{
    return amplituda;
}

void SensorAngleRotate::setAmplituda(float newAmplituda)
{
    amplituda = newAmplituda;
}

void SensorAngleRotate::setKoeff(float newKoeff)
{
    koeff = newKoeff;
    setAngleWheelsRotate(currentAngle);
}

bool SensorAngleRotate::saveParameters()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    settings.setValue(DIR_CALIBRATION
                      SUBDIR_WHEEL_ANGLE
                      KEY_WHEEL_KOEFF,
                      koeff);
    settings.setValue(DIR_CALIBRATION
                      SUBDIR_WHEEL_ANGLE
                      KEY_WHEEL_DELTA,
                      delta);
    settings.setValue(DIR_CALIBRATION
                      SUBDIR_WHEEL_ANGLE
                      KEY_WHEEL_AMPLITUDA,
                      amplituda);

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        qDebug() << "Коеффициент пропорциональности и смещения датчика угла поворота успешно сохранены!";
        return true;
    }

    qDebug() << "Ошибка сохранения коеффициента пропорциональности и смещения датчика угла поворота!";
    return false;
}

bool SensorAngleRotate::readParameters()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    float k = settings.value(DIR_CALIBRATION
                             SUBDIR_WHEEL_ANGLE
                             KEY_WHEEL_KOEFF,
                             DEFAULT_WHEEL_KOEFF).toFloat();
    float d = settings.value(DIR_CALIBRATION
                             SUBDIR_WHEEL_ANGLE
                             KEY_WHEEL_DELTA,
                             DEFAULT_WHEEL_DELTA).toFloat();
    float a = settings.value(DIR_CALIBRATION
                             SUBDIR_WHEEL_ANGLE
                             KEY_WHEEL_DELTA,
                             DEFAULT_WHEEL_AMPLITUDA).toFloat();


    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        koeff = k;
        delta = d;
        amplituda = a;
        qDebug() << "Коэффициент пропорциональности и смещения датчика угла поворота успешно загружены !";
        return true;
    }

    qDebug() << "Ошибка загрузки коэффициента пропорциональности и смещения датчика угла поворота !";
    return false;
}

void SensorAngleRotate::setDelta(float newDelta)
{
    delta = newDelta;
    setAngleWheelsRotate(currentAngle);
}

int SensorAngleRotate::getCurrentAngle() const
{
    return currentAngle;
}

float SensorAngleRotate::getAngleWheelsRotate() const
{
    return angleWheelsRotate;
}

void SensorAngleRotate::readData()
{
#ifdef Q_OS_LINUX
    qDebug() << "-----------------------------------------------";
    if (this->deviceRegAdress == -1) {
        qDebug() << "[SlaveController::loop()] deviceRegAdress == -1";
        return;
    }

    const int SIGN_PLUS  = 101; // константа кодирующая положительное значение
    const int SIGN_MINUS = 21;  // константа кодирующая отрицательное значение

    const uint8_t COMM_GETH = 20; // команда на чтение старшего разряда
    const uint8_t COMM_GETL = 21; // команда на чтение младшего разряда
    const uint8_t COMM_GETS = 22; // команда на чтение знака

    int receivedDataH = wiringPiI2CReadReg8(this->deviceRegAdress, COMM_GETH);
    int receivedDataL = wiringPiI2CReadReg8(this->deviceRegAdress, COMM_GETL);
    int receivedDataS = wiringPiI2CReadReg8(this->deviceRegAdress, COMM_GETS);

    // проверка валидности принятого идентификатора знака
    bool isValidSign = (receivedDataS == SIGN_PLUS) || (receivedDataS == SIGN_MINUS);

    // Если принятые данные не в диапазоне, значит произошла ошибка
    if(receivedDataH > 100 ||
       receivedDataL > 100 ||
       !isValidSign ) {

        qDebug() << "void SensorAngleRotate::readData() ОШИБКА ПОЛУЧЕНИЯ ДАННЫХ С ДАТЧИКА УГЛА ПОВОРОТА !!!!";
        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read H: " << receivedDataH;
        qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read L: " << receivedDataL;
        return;
    }

    int rd = abs(receivedDataH) * 100 + abs(receivedDataL);

    if(receivedDataS == SIGN_PLUS) {
        // ничего
    }
    if(receivedDataS == SIGN_MINUS) {
        rd = -rd;
    }

    currentAngle = ((float)rd)/600.0 * 360.0;
    setAngleWheelsRotate(currentAngle);

    qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read H: " << receivedDataH;
    qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read L: " << receivedDataL;
    qDebug() << "Slave" << QString::number(this->hexAdress).toLocal8Bit() << "read summ: " << rd;

#else
    #ifdef Q_OS_WIN
        qDebug() << "void SensorAngleRotate::updateData()";
    #endif
#endif
}

