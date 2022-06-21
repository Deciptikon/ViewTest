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

    void init(const int secDeltaTime);

    int getMSecDeltaTime() const;

    void restateRelay();

signals:
    void sendCommandToSlave14(const int &comm);

    void pathChanged(const ListVector &path);

    void signalAppPointToPath(const QVector2D &vec);

    void signalAppPointToPathAndRemoveFirst(const QVector2D &vec);

    void keyPointsChanged(const ListVector &keyPoints);

    void sendPointAToDraw(const QVector2D &vec);

    void sendDirectToDraw(const QVector2D &dir);

public slots:
    void loop();

    void readFromGPS(const double &x, const double &y);

    void readFromGyroAndAccel(const QVector3D &accel, const QVector3D &gyro);

    void addKeyPoint(const QVector2D &point);

    void acceptDriveMode(const QVariant &mode);

    void slotCreateQuadroKeyPoint();

    void slotSetPointA();

    void slotSetPointB();

private slots:
    //

private:
    void driveKeyPoint();
    void driveParallel();
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
};

#endif // AUTOPILOT_H
