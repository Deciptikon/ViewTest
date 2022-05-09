#include "appsettings.h"

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    qDebug() << "регистрация приложения";
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    qDebug() << "получение настроек";
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
}

QString AppSettings::getValueString(const QString key)
{
    return settings.value(key, false).toString();
}

qreal AppSettings::getValueQreal(const QString key)
{
    return settings.value(key, false).toReal();
}

int AppSettings::getValueInt(const QString key)
{
    return settings.value(key, false).toInt();
}

bool AppSettings::getValueBool(const QString key)
{
    return settings.value(key, false).toBool();
}

void AppSettings::setValue(QString key, QVariant value)
{
    settings.setValue(key, value);
    settings.sync();
    if(settings.status() == QSettings::NoError) {
        emit valueChanged();
    }
}
