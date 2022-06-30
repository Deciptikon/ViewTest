#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <QObject>
#include <QDebug>
#include <QVector3D>
#include <QtMath>

#ifdef Q_OS_WIN
    //пока здесь ничего нет
#else
    #include <wiringPi.h>
    #include <wiringPiI2C.h>
#endif

class Gyroscope : public QObject
{
    Q_OBJECT
public:
    explicit Gyroscope(QObject *parent = nullptr);

    /// Регистрирует устройство в системе и настраивает его на обмен данными.
    void init();

    /// Обновляет данные с сенсоров (читает из регистров и записывает в
    /// приватные поля класса).
    void updateData();

    /// Получает текущие отмасштабированные данные за вычетом состояния покоя.
    QVector3D getData() const;

    /// Устанавливает новый вектор состояния покоя.
    void setZeroData(const QVector3D &vec);

    /// Возвращает текущий вектор состояния покоя.
    const QVector3D &getZeroData() const;

    /// Читает последний сохраненный в настройках приложения вектор состояния покоя
    /// и возвращает результат успешности процесса чтения.
    bool readZeroData();

    /// Сохраняет текущий вектор состояния покоя в настройки приложения
    /// и возвращает результат успешности процесса сохранения.
    bool saveZeroData();

    /// Читает последний сохраненный коэффициент пропорциональности
    /// из настроек приложения и возвращает результат успешности процесса чтения.
    bool readCoefficient();

    /// Сохраняет коэффициент пропорциональности в настройки приложения и
    /// возвращает результат успешности процесса сохранения.
    bool saveCoefficient();

    /// Возвращает текущий коэффициент пропорциональности\масштабирования
    /// данных полученных с сенсора.
    float getCoefficient() const;

    /// Устанавливает новый коэффициент пропорциональности.
    void setCoefficient(float newCoefficient);

signals:

private:
    // текущие полученные данные
    QVector3D data;

    // калибровка состояния покоя
    QVector3D zeroData{ 0, 0, 0};

    // коэффициент масштабирования
    float coefficient = 1;

    //идентификатор устройства (гироскопа)
    int deviceGyro;
};

#endif // GYROSCOPE_H
