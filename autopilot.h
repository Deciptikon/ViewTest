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


public slots:
    void loop();

    void readFromGPS(const double &x, const double &y);

    void addKeyPoint(const QVector2D &point);

    void acceptDriveMode(const QVariant &mode);

    void slotCreateQuadroKeyPoint();

private slots:
    //

private:
    int msecDeltaTime;

    bool stateRelay; //состояние реле подруливающего устройства

    bool isOrigin = false;
    double xOrigin = 0;//локальныая система координат
    double yOrigin = 0;

    QVector2D direction;//направление перемещения
    QVector2D directionToPoint;

    QList<QVector2D> path2D;

    QList<QVector2D> listPoint2D;
};

#endif // AUTOPILOT_H
