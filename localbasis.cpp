#include "localbasis.h"
#include <QSettings>

LocalBasis::LocalBasis()
{
    m_localX = {1, 0, 0};
    m_localY = {0, 1, 0};
    m_localZ = {0, 0, 1};
}

void LocalBasis::debug()
{
    qDebug() << "------------------------------------------";
    qDebug() << "----{localX}:" << m_localX;
    qDebug() << "----{localY}:" << m_localY;
    qDebug() << "----{localZ}:" << m_localZ;
    qDebug() << "------------------------------------------";
}

bool LocalBasis::saveBasis()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    settings.setValue(DIR_CALIBRATION
                      KEY_X_LOCALX,
                      m_localX.x());
    settings.setValue(DIR_CALIBRATION
                      KEY_Y_LOCALX,
                      m_localX.y());
    settings.setValue(DIR_CALIBRATION
                      KEY_Z_LOCALX,
                      m_localX.z());

    settings.setValue(DIR_CALIBRATION
                      KEY_X_LOCALY,
                      m_localY.x());
    settings.setValue(DIR_CALIBRATION
                      KEY_Y_LOCALY,
                      m_localY.y());
    settings.setValue(DIR_CALIBRATION
                      KEY_Z_LOCALY,
                      m_localY.z());

    settings.setValue(DIR_CALIBRATION
                      KEY_X_LOCALZ,
                      m_localZ.x());
    settings.setValue(DIR_CALIBRATION
                      KEY_Y_LOCALZ,
                      m_localZ.y());
    settings.setValue(DIR_CALIBRATION
                      KEY_Z_LOCALZ,
                      m_localZ.z());

    settings.sync(); // синхронизируемся и получаем статус

    if (settings.status() == QSettings::NoError) {
        qDebug() << "Локальный базис успешно сохранен";
        return true;
    }

    qDebug() << "Ошибка сохранения локального базиса";
    return false;
}

bool LocalBasis::readBasis()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    float xLocX = settings.value(DIR_CALIBRATION
                                 KEY_X_LOCALX,
                                 DEFAULT_X_LOCALX).toFloat();
    float yLocX = settings.value(DIR_CALIBRATION
                                 KEY_Y_LOCALX,
                                 DEFAULT_Y_LOCALX).toFloat();
    float zLocX = settings.value(DIR_CALIBRATION
                                 KEY_Z_LOCALX,
                                 DEFAULT_Z_LOCALX).toFloat();

    float xLocY = settings.value(DIR_CALIBRATION
                                 KEY_X_LOCALY,
                                 DEFAULT_X_LOCALY).toFloat();
    float yLocY = settings.value(DIR_CALIBRATION
                                 KEY_Y_LOCALY,
                                 DEFAULT_Y_LOCALY).toFloat();
    float zLocY = settings.value(DIR_CALIBRATION
                                 KEY_Z_LOCALY,
                                 DEFAULT_Z_LOCALY).toFloat();

    float xLocZ = settings.value(DIR_CALIBRATION
                                 KEY_X_LOCALZ,
                                 DEFAULT_X_LOCALZ).toFloat();
    float yLocZ = settings.value(DIR_CALIBRATION
                                 KEY_Y_LOCALZ,
                                 DEFAULT_Y_LOCALZ).toFloat();
    float zLocZ = settings.value(DIR_CALIBRATION
                                 KEY_Z_LOCALZ,
                                 DEFAULT_Z_LOCALZ).toFloat();

    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) {
        m_localX = {xLocX, yLocX, zLocX};
        m_localY = {xLocY, yLocY, zLocY};
        m_localZ = {xLocZ, yLocZ, zLocZ};
        qDebug() << "Локальный базис успешно загружен !";
        return true;
    }

    qDebug() << "Ошибка загрузки локального базиса !";
    return false;
}

void LocalBasis::setLocalX(const QVector3D &vec)
{
    if(vec.isNull()) {
        return;
    }

    m_localX = vec.normalized();
    m_localY = QVector3D::crossProduct(m_localX, m_localZ).normalized();
}

const QVector3D &LocalBasis::localX() const
{
    return m_localX;
}

const QVector3D &LocalBasis::localY() const
{
    return m_localY;
}

const QVector3D &LocalBasis::localZ() const
{
    return m_localZ;
}

// преобразование вектора vec в локальную систему координат
// заданную тремя базисными векторами m_localX, m_localY, m_localZ
// имеющими длину = 1
QVector3D LocalBasis::toLocalBasis(const QVector3D &vec)
{
    QVector3D loc;

    loc.setX(QVector3D::dotProduct( m_localX, vec));
    loc.setY(QVector3D::dotProduct( m_localY, vec));
    loc.setZ(QVector3D::dotProduct( m_localZ, vec));

    return loc;
}

void LocalBasis::setLocalZ(const QVector3D &vec)
{
    if(vec.isNull()) {
        return;
    }

    m_localZ = vec.normalized();
    m_localY = QVector3D::crossProduct(m_localX, m_localZ).normalized();
}
