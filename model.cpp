#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor model";
    testData = ("|");

    timerGPS = new QTimer();
    connect(timerGPS, SIGNAL(timeout()), this, SLOT(slotUpdateTimerGPS()) );
    timerGPS->start(5000);

    timerI2C = new QTimer();
    connect(timerI2C, SIGNAL(timeout()), this, SLOT(slotUpdateTimerI2C()) );
    timerI2C->start(4150);
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
