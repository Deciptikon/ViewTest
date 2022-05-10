#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor model";
    testData = ("|");

    timerGPS = new QTimer();
    connect(timerGPS, SIGNAL(timeout()), this, SLOT(slotUpdateTimerGPS()) );
    timerGPS->start(5000);
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
