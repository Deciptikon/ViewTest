#ifndef SENSORANGLEROTATE_H
#define SENSORANGLEROTATE_H

#include <QObject>
#include <QDebug>

#ifdef Q_OS_WIN
    //пока здесь ничего нет
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

class SensorAngleRotate : public QObject
{
    Q_OBJECT
public:
    explicit SensorAngleRotate(QObject *parent = nullptr);

    /// Регистрирует устройство в системе и настраивает его на обмен данными.
    void init(int hexAdress);

    /// Возвращает текущий угол поворота рулевого колеса
    int getCurrentAngle() const;

    /// Возвращает текущий угол поворота ведущих колёс (в радианах)
    float getAngleWheelsRotate() const;

    /// Устанавливает новое смещение угла.
    void setDelta(float newDelta);

    /// Устанавливает новый коеффициент пропорциональности.
    void setKoeff(float newKoeff);

    /// Сохраняет текущие параметры (смещение и коеффициент) в настройки приложения
    bool saveParameters();

    /// Читает последние сохраненные параметры (смещение и коеффициент) из настроек приложения
    bool readParameters();

    void setAmplituda(float newAmplituda);

    float getAmplituda() const;

signals:


public slots:
    /// Обновляет данные с сенсоров (читает из регистров и записывает в
    /// приватные поля класса).
    void readData();

    /// Слот, передает данные на устройство по i2c.
    void writeData(const int &data);

private:
    /// Вычисляет угол направления колес на основе угла полученного
    /// с датчика угла поворота рулевого колеса.
    void setAngleWheelsRotate(int angle);

    int hexAdress;
    int deviceRegAdress;

    /// Текущий угол полученый от датчика.
    int currentAngle = 0;

    /// Угол поворота колес найденный при помощи линейного
    /// закона.
    float angleWheelsRotate = 0;

    /// Коэффициент пропорциональности в линейном преобразовании.
    float koeff = 1.0;

    /// Величина смещения в линейном преобразовании.
    float delta = 0.0;

    /// Максимальное отклонение от состояния равновесия (прямолинейного движения)
    float amplituda = 1.0;


};

#endif // SENSORANGLEROTATE_H
