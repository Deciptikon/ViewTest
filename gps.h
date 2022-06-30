#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QtMath>
#include <QSettings>

#include <QTimer>
//#include <QElapsedTimer>

#include "constants.h"


const double RADIUS_EARTH = 6371000;
const double MAX_LONGITUDE = 85.0511288;

class GPS : public QObject
{
    Q_OBJECT
public:
    explicit GPS(QObject *parent = nullptr);

    /// Устанавливает период обновления !!!пока не используется!!!
    void setMsecUpdate(size_t msec);

signals:
    /// Сигнал передающий текущее положение в Декартовых координатах
    /// (в проекции Меркатора).
    void updatePositionXY(const double &x, const double &y);
    /// Сигнал передающий текущее положение в угловых координатах.
    void updatePositionLatLon(const double &lat, const double &lon);

    /// Сигнал с данными готовыми для чтения.
    void readyRead(const QByteArray &bytes);
    /// Сигнал начала парсинга сообщения.
    void parseMessage();

    /// Сигнал о рабочем состоянии GPS
    void gpsOn();
    /// Сигнал о не рабочем состоянии GPS
    void gpsOff();

public slots:
    /// Открывает соединение через последовательное соединение
    /// и настраивает его, а так же формирует основные зависимости
    /// для корректной работы.
    void init();
    /// Записывает в последовательный порт массив данных.
    void write(const QByteArray &bytes);


private slots:
    /// Слот читает из последовательного порта.
    void readPort();
    /// Слот парсит данные в соответствии с UBX протоколом.
    void ubxParser();
    /// Слот открывает последовательный порт.
    void openPort();

private:
    /// процедура преобразует угловые координаты на сфере в
    /// Декартовы координаты (в проекции Меркатора)
    void latLonToXY(double lat, double lon);

    QSerialPort* serial;

    QByteArray messageBuffer;
    QByteArray messageCurrent;

    double lat;
    double lon;

    double x,y;

    size_t msecUpdate;

    bool enableGPS = true;
};

#endif // GPS_H
