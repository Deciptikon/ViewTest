#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <QObject>

class Accelerometer : public QObject
{
    Q_OBJECT
public:
    explicit Accelerometer(QObject *parent = nullptr);

signals:

};

#endif // ACCELEROMETER_H
