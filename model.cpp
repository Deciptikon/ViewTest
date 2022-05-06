#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor model";
    testData = ("|");
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
