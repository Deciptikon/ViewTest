#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include <QDebug>
#include <QtMath>
#include <QList>
#include <QVector2D>
#include <QVector3D>

#include "DriveMode.h"

typedef QList<QVector2D> ListVector;

class Autopilot : public QObject
{
    Q_OBJECT
public:
    explicit Autopilot(QObject *parent = nullptr);

    /// Чтение основных настроек и установка необходимых зависимостей.
    void init(const int secDeltaTime);

    /// Функция возвращает количество интервал обновления по таймеру
    /// в миллисекундах.
    int getMSecDeltaTime() const;

    /// Функция изменяет флаг состояния реле и отправляет команду
    /// в подруливающее устройство.
    void restateRelay();

signals:
    /// Сигнал, отправляет команду на контроллер подруливающего устройства.
    void sendCommandToSlave14(const int &comm);

    /// Сигнал, отправляет ссылку на траекторию перемещения при её изменении
    /// (удалении или добавлении точки).
    void pathChanged(const ListVector &path);

    /// Сигнал, отправляет точку пути если она была добавлена в конец пути.
    void signalAppPointToPath(const QVector2D &vec);

    /// Сигнал, отправляет точку пути если она была добавлена в конец пути
    /// и удаляет первую точку (таким образом регулируя длину пути).
    void signalAppPointToPathAndRemoveFirst(const QVector2D &vec);

    /// Сигнал, отправляет список всех ключевых точек если он был изменён
    /// (была добавлена или удалена точка).
    void keyPointsChanged(const ListVector &keyPoints);

    /// Сигнал, отправляет точку А для рисования в QML.
    void sendPointAToDraw(const QVector2D &vec);

    /// Сигнал, отправляет направление посчитанное от точки А до точки В.
    void sendDirectToDraw(const QVector2D &dir);

public slots:
    /// Основной цикл автопилотирования.
    void loop();

    /// Слот, принимает данные от GPS (в проекции Меркатора).
    void readFromGPS(const double &x, const double &y);

    /// Слот, принимает данные от Гироскопа и Акселерометра.
    void readFromGyroAndAccel(const QVector3D &accel, const QVector3D &gyro);

    /// Слот, принимает данные от датчика угла поворота рулевого колеса (в градусах)
    void readFromSensorAngleRotation(const float &angle);

    /// Слот, принимает ключевую точку полученную из QML.
    void addKeyPoint(const QVector2D &point);

    /// Слот, принимает от QML режим вождения DriveMode приведенный к QVariant.
    void acceptDriveMode(const QVariant &mode);

    /// Слот, формирует четыре точки в углах квадрата с одной из сторон
    /// сонаправленных с направлением движения.
    void slotCreateQuadroKeyPoint();

    /// Слот, устанавливает текущую координату в качестве точки А.
    void slotSetPointA();

    /// Слот, устанавливает текущую кооддинату в качестве точки В.
    void slotSetPointB();

private slots:
    //

private:
    /// Алгоритм вождения по ключевым точкам.
    void driveKeyPoint();

    /// Алгоритм вождения вдоль параллельных прямых.
    void driveParallel();

    /// Алгоритм вождения по спирали (вдоль замкнутой кромки)
    // void driveSpiral();

    int msecDeltaTime;

    bool stateRelay; //состояние реле подруливающего устройства

    bool isOrigin = false;
    double xOrigin = 0;//локальныая система координат
    double yOrigin = 0;


    QVector2D direction;//направление перемещения
    QVector2D directionToPoint;

    QList<QVector2D> path2D;//trajectory

    QList<QVector2D> listPoint2D;//keypoints

    DriveMode::State currentDriveMode = DriveMode::NONE_MODE;

    float widthBetweenLines;// ширина между линиями
    QVector2D pointA{0,0};
    QVector2D pointB{0,0};
    QVector2D dir; // направление линий параллельного вождения
    QVector2D orthoDir;//{dir.y(), -dir.x()};

    float angleWheelsRotate;// угол поворота колёс
};

#endif // AUTOPILOT_H
