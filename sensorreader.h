#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include "accelerometer.h"
#include "gyroscope.h"
#include "localbasis.h"
#include "sensoranglerotate.h"


class SensorReader : public QObject
{
    Q_OBJECT
public:
    explicit SensorReader(QObject *parent = nullptr);

    /// Регистрирует устройство в системе и настраивает его на обмен данными,
    /// устанавливает интервал принудительного обновления данных от
    /// акселерометра и гироскопа.
    void init(const int msec=100);

    /// Возвращает текущий интервал принудителных обновлений.
    int getMsecDeltaTime() const;

    /// Акселерометр, содержит функции и члены для получения данных
    /// с физического устройства по каналу I2C и калибровки нулевого положения,
    /// а так же процедуры чтения и записи откалиброванных значений в настройки приложения.
    Accelerometer Accelerometer;

    /// Гироскоп, содержит функции и члены для получения данных
    /// с физического устройства по каналу I2C и калибровки нулевого положения,
    /// а так же процедуры чтения и записи откалиброванных значений в настройки приложения.
    Gyroscope     Gyroscope;

    SensorAngleRotate AngleRotate;

signals:
    /// Сигнал передающий текущие данные полученные с гироскопа и
    /// акселерометра в систему (автопилоту, QML и ...).
    void updateDataSens(const QVector3D &accel, const QVector3D &gyro);

    /// Сигнал о успешной калибровки состояния покоя акселерометра для QML.
    void signalCalibrateZeroPointAccelerometerIsDone();
    /// Сигнал о успешной калибровки состояния покоя гироскопа для QML.
    void signalCalibrateZeroPointGyroscopeIsDone();

    /// Сигнал о успешной калибровки оси Z гироскопа для QML.
    void signalCalibrateZAxisGyroscopeIsDone();
    /// Сигнал о успешной калибровки оси X акселерометра для QML.
    void signalCalibrateXAxisAccelerometerIsDone();

public slots:
    /// Основной цикл обновления данных с акселерометра и гироскопа.
    void loop();

    /// Слот запуска калибровки состояния покоя акселерометра,
    /// с длительностью в миллисекундах.
    void slotCalibrateZeroPointAccelerometer(const int &msec);
    /// Слот запуска калибровки состояния покоя гироскопа,
    /// с длительностью в миллисекундах.
    void slotCalibrateZeroPointGyroscope(const int &msec);

    /// Слот запуска\остановки калибровки оси Z гироскопа.
    void slotCalibrateZAxisGyroscope();
    /// Слот запуска\остановки калибровки оси X акселерометра,
    void slotCalibrateXAxisAccelerometer();

private:
    /// Процедура калибровки состояния покоя акселерометра.
    void calibrateZeroPointAccelerometer();
    /// Процедура калибровки состояния покоя гироскопа.
    void calibrateZeroPointGyroscope();

    /// Процедура калибровки оси Z гироскопа.
    void calibrateZAxisGyroscope();
    /// Процедура калибровки оси Х акселерометра.
    void calibrateXAxisAccelerometer();


    int msecDeltaTime = 100;

    // локальная система координат, определяемая во время калибровок
    LocalBasis localBasis;


    // переменные и флаги содержащие данные калибровки
    bool flagCalibrateZeroPointAccelerometer = false;
    QVector3D dataCalibrateZeroPointAccelerometer;
    int numCalibrateZeroPointAccelerometer;

    bool flagCalibrateZeroPointGyroscope = false;
    QVector3D dataCalibrateZeroPointGyroscope;
    int numCalibrateZeroPointGyroscope;


    bool flagCalibrateZAxisGyroscope = false;
    QVector3D dataCalibrateZAxisGyroscope;
    int numCalibrateZAxisGyroscope;
    QElapsedTimer elapsedTimer;
    float elapsedTime;
    float gyrosCoefficient = 1;// коеффициент соответствия показателей

    bool flagCalibrateXAxisAccelerometer = false;
    QVector3D dataCalibrateXAxisAccelerometer;
    int numCalibrateXAxisAccelerometer;
    float accelCoefficient = 1;// коеффициент соответствия показателей

};

#endif // SENSORREADER_H
