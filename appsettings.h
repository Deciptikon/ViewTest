#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QGuiApplication>
#include <QDebug>

#define ORGANIZATION_NAME    "Deciptikon"
#define ORGANIZATION_DOMAIN  "vk.com/deciptikon"
#define APPLICATION_NAME     "AgroSlave"

#define TEST_NAME            "testName"

class AppSettings : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings(QObject *parent = nullptr);

    Q_INVOKABLE QString getValueString(const QString key);
    Q_INVOKABLE qreal   getValueQreal(const QString key);
    Q_INVOKABLE int     getValueInt(const QString key);
    Q_INVOKABLE bool    getValueBool(const QString key);

signals:
    void valueChanged();

public slots:
    void setValue(const QString key, QVariant value);

private:
    QSettings settings;
};

#endif // APPSETTINGS_H
