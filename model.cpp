#include "model.h"
#include <QString>
// константы (в основном ключи и директории реестра)
#include "constants.h"

Model::Model(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor model";
    testData = ("|");

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    QString val = QString( settings.value(DIR_STATUSBAR KEY_COLOR_STATUSBAR, "").toString() );
    qDebug() << val;
    QColor col = QColor(val);
    qDebug() << col;
    setColorStatusBar(col);


//    // таймеры и связи для демонстрации
//    timerGPS = new QTimer();
//    connect(timerGPS, SIGNAL(timeout()), this, SLOT(slotUpdateTimerGPS()) );
//    timerGPS->start(5000);

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

void Model::acceptCoordXY(const double &x, const double &y)
{
    QVector2D vec;
    if( xorig == 0  &&  yorig == 0  &&  x != 0  &&  y != 0) {
        qDebug() << "SET ORIG POSITION";
        xorig = x;
        yorig = y;
    }
    float xf = x - xorig;
    float yf = y - yorig;
    vec.setX(xf);
    vec.setY(yf);
    if(lenpath<1000) {
        lenpath++;
        emit signalAppPointToPathQML(vec);
    } else {
        emit signalAppPointToPathAndRemoveFirstQML(vec);
    }
}

void Model::setDriveModeFromQML(QVariant mode)
{
    //qDebug() << "ВЫБРАН РЕЖИМ ВОЖДЕНИЯ " << mode.toUInt();

    if(mode.toUInt() == DriveMode::NONE_MODE) {
        qDebug() << "ВЫБРАН РЕЖИМ ВОЖДЕНИЯ NONE";
    }
    if(mode.toUInt() == DriveMode::KEYPOINTS_MODE) {
        qDebug() << "ВЫБРАН РЕЖИМ ВОЖДЕНИЯ KEYPOINTS_MODE";
    }
    if(mode.toUInt() == DriveMode::PARALLEL_MODE) {
        qDebug() << "ВЫБРАН РЕЖИМ ВОЖДЕНИЯ PARALLEL_MODE";
    }
    if(mode.toUInt() == DriveMode::SPIRAL_MODE) {
        qDebug() << "ВЫБРАН РЕЖИМ ВОЖДЕНИЯ SPIRAL_MODE";
    }

    // передаем режим в автопилот...
}

void Model::slotGPSon()
{
    emit signalStateGPStoQML(true);
}

void Model::slotGPSoff()
{
    emit signalStateGPStoQML(false);
}

// таймеры и состояния для демонстрации
//void Model::slotUpdateTimerGPS()
//{
//    stateGPS = !stateGPS;
//    qDebug() << "stateGPS =" << stateGPS;
//    emit signalStateGPStoQML(stateGPS);
//}

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

const QColor &Model::colorStatusBar() const
{
    return m_colorStatusBar;
}

void Model::setColorStatusBar(const QColor &newColorStatusBar)
{
    if (m_colorStatusBar == newColorStatusBar)
        return;
    m_colorStatusBar = newColorStatusBar;
    emit colorStatusBarChanged();
}
