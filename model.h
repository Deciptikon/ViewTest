#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QTimer>

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    Q_INVOKABLE void testMetod(QString str);

signals:
    void signalSendToQML(QString str);

    void signalStateGPStoQML(bool state);
    void signalStateI2CtoQML(bool state);

public slots:
    void slotTakeFromQML(QString str);

    void slotUpdateTimerGPS();
    void slotUpdateTimerI2C();

private:
    QString testData;

    QTimer *timerGPS;
    bool stateGPS = false;

    QTimer *timerI2C;
    bool stateI2C = false;
};

#endif // MODEL_H
