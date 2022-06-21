#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <QObject>

class Gyroscope : public QObject
{
    Q_OBJECT
public:
    explicit Gyroscope(QObject *parent = nullptr);

signals:

};

#endif // GYROSCOPE_H
