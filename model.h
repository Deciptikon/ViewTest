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

public slots:
    void slotTakeFromQML(QString str);

    void slotUpdateTimerGPS();

private:
    QString testData;

    QTimer *timerGPS;
    bool stateGPS = false;
};

#endif // MODEL_H
