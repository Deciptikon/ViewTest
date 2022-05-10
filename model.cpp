#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor model";
    testData = ("|");

    // таймеры и связи для демонстрации
    timerGPS = new QTimer();
    connect(timerGPS, SIGNAL(timeout()), this, SLOT(slotUpdateTimerGPS()) );
    timerGPS->start(5000);

    timerI2C = new QTimer();
    connect(timerI2C, SIGNAL(timeout()), this, SLOT(slotUpdateTimerI2C()) );
    timerI2C->start(4150);

    timerGyro = new QTimer();
    connect(timerGyro, SIGNAL(timeout()), this, SLOT(slotUpdateTimerGyro()) );
    timerGyro->start(4823);

    timerCam = new QTimer();
    connect(timerCam, SIGNAL(timeout()), this, SLOT(slotUpdateTimerCam()) );
    timerCam->start(3186);
}

void Model::testMetod(QString str)
{
    qDebug() << "Из qml получено:" << str;
}

void Model::slotTakeFromQML(QString str)
{
    qDebug() << "Слот вызван из QML";
    if(testData == str) {
        return;
    }
    testData.append(str);
    emit signalSendToQML(testData);
}

// таймеры и состояния для демонстрации
void Model::slotUpdateTimerGPS()
{
    stateGPS = !stateGPS;
    qDebug() << "stateGPS =" << stateGPS;
    emit signalStateGPStoQML(stateGPS);
}

void Model::slotUpdateTimerI2C()
{
    stateI2C = !stateI2C;
    qDebug() << "stateI2C =" << stateI2C;
    emit signalStateI2CtoQML(stateI2C);
}

void Model::slotUpdateTimerGyro()
{
    stateGyro = !stateGyro;
    qDebug() << "stateGyro =" << stateGyro;
    emit signalStateGyrotoQML(stateGyro);
}

void Model::slotUpdateTimerCam()
{
    stateCam = !stateCam;
    qDebug() << "stateCam =" << stateCam;
    emit signalStateCamtoQML(stateCam);
}
