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

#include "TypeEdit.h"


#define ORGANIZATION_NAME    "Deciptikon"
#define ORGANIZATION_DOMAIN  "vk.com/deciptikon"
#define APPLICATION_NAME     "AgroSlave"

#define TEST_NAME            "testName"

//enum TypeEdit {
//    BOOL_EDIT,
//    DOUBLE_EDIT,
//    STRING_EDIT
//};

struct SettingsData {
    QString  key;
    QString  title;
    TypeEdit::State typeEdit;
};

class AppSettings : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
            TitleRole = Qt::UserRole + 1,
            ValueRole,
            TypeEditRole
        };

    explicit AppSettings(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //Q_INVOKABLE void add();

    Q_INVOKABLE QVariant getValue(const QString key);
    Q_INVOKABLE QString  getValueString(const QString key);
    Q_INVOKABLE qreal    getValueQreal(const QString key);
    Q_INVOKABLE int      getValueInt(const QString key);
    Q_INVOKABLE bool     getValueBool(const QString key);

signals:
    void settingsChanged();

public slots:
    void setValue(const QString key, QVariant value);

private:
    QList< SettingsData > listKeys; // список пар <ключ, заголовок>
};

#endif // APPSETTINGS_H
