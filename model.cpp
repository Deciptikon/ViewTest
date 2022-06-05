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

    //отключаем видимость кнопки как только выбрана точка B
    connect(this, SIGNAL(signalSetPointB()), this, SLOT(disableButtonAB()) );
}

void Model::setDriveModeFromQML(QVariant mode)
{
    // передаем режим в автопилот...
    if(mode.toUInt() == DriveMode::NONE_MODE ||
       mode.toUInt() == DriveMode::KEYPOINTS_MODE ) {
        //disableButtonAB();
        isEnableAB = false;
    } else {
        isEnableAB = true;
    }
    emit sendDriveMode(mode);
}

bool Model::isVisibleButtonAB()
{
    return isEnableAB == true;
}

bool Model::isEnableButtonAB()
{
    return isEnableAB == true;
}

void Model::slotGPSon()
{
    emit signalStateGPStoQML(true);
}

void Model::slotGPSoff()
{
    emit signalStateGPStoQML(false);
}

void Model::slotUpdateTimerI2C()
{
    stateI2C = !stateI2C;
    //qDebug() << "stateI2C =" << stateI2C;
    emit signalStateI2CtoQML(stateI2C);
}

void Model::slotUpdateTimerGyro()
{
    stateGyro = !stateGyro;
    //qDebug() << "stateGyro =" << stateGyro;
    emit signalStateGyrotoQML(stateGyro);
}

void Model::slotUpdateTimerCam()
{
    stateCam = !stateCam;
    //qDebug() << "stateCam =" << stateCam;
    emit signalStateCamtoQML(stateCam);
}

void Model::slotCommandToSlave14(int comm)
{
    emit signalCommandToSlave14(comm);
}

void Model::slotAppPointToPath(const QVector2D &vec)
{
    QVector2D v = vec;
    qDebug() << "slotAppPointToPath";
    emit signalAppPointToPathQML(v);
}

void Model::slotAppPointToPathAndRemoveFirst(const QVector2D &vec)
{
    QVector2D v = vec;
    qDebug() << "slotAppPointToPathAndRemoveFirst";
    emit signalAppPointToPathAndRemoveFirstQML(v);
}

void Model::acceptKeyPoints(const ListVector &keyPoints)
{
    this->keyPoints = keyPoints;
    emit keyPointsToQML(this->keyPoints);
}

void Model::addKeyPointFromQML(const QVector2D point)
{
    emit sendKeyPointForAdding(point);
}

void Model::addPointAToQML(const QVector2D &point)
{
    QVector2D p = point;
    emit sendPointAToQML(p);
}

void Model::addDirectToQML(const QVector2D &dir)
{
    QVector2D d = dir;
    emit sendDirectToQML(d);
}

void Model::slotRotateFromAutopilot(const int &msec)
{
    int m = msec;

    if(m > 0) {
        emit signalRotateLeftToQML();
    }
    if(m < 0) {
        emit signalRotateRightToQML();
    }

    count++;
    emit signalCountToQML(count);
}

void Model::disableButtonAB()
{
    isEnableAB = false;
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
