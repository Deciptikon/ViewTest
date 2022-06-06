#ifndef TESTGPS_H
#define TESTGPS_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QtMath>

class TestGPS : public QObject
{
    Q_OBJECT
public:
    explicit TestGPS(QObject *parent = nullptr);

signals:
    void updatePositionXY(const double &x, const double &y);

public slots:
    void read();


private:

    double x=0, y=0;


};

#endif // TESTGPS_H
