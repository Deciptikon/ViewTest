#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QAbstractListModel>
#include <QSettings>
#include <QGuiApplication>
#include <QDebug>
#include <QColor>
#include <QList>
#include <QPair>

#include "constants.h"
#include "TypeEdit.h"

//using namespace constants;

//#define ORGANIZATION_NAME    "Deciptikon"
//#define ORGANIZATION_DOMAIN  "vk.com/deciptikon"
//#define APPLICATION_NAME     "AgroSlave"

//#define TEST_NAME            "testName"

struct SettingsData {
    QString  key;
    QString  title;
    TypeEdit::State typeEdit;
    QString  defaultValue;
};

class AppSettings : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {// роли для использования в qml
            TitleRole = Qt::UserRole + 1,
            ValueRole,
            TypeEditRole,
        };

    explicit AppSettings(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;


    void checkAndRestoreSettings();

signals:
    void settingsChanged();

public slots:
    //void setValue(const QString key, QVariant value);

private:
    QList< SettingsData > listKeys; // список пар <ключ, заголовок, тип редактирования>
};

#endif // APPSETTINGS_H
