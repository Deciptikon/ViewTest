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

struct SettingsData {
    /// Ключ для записи настроек через QSettings
    /// (не доступно пользователю напрямую).
    QString  key;
    /// Наименование, видимое для пользователя.
    QString  title;
    /// Тип редактирования настройки (влияет на выбор
    /// делегата в окне редактирования настроек).
    TypeEdit::State typeEdit;
    /// Значение по умолчанию, для использования через
    /// QSettings и востановления настроек.
    QString  defaultValue;
};

class AppSettings : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {// роли для использования в qml
        /// Название настройки.
        TitleRole = Qt::UserRole + 1,
        /// Редактируемая величина.
        ValueRole,
        /// Тип редактирования.
        TypeEditRole,
    };

    explicit AppSettings(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    /// Сигнал о изменении настроек.
    void settingsChanged();

public slots:
    /// Функция проверяет необходимость востановления настроек приложения
    /// и востанавливает их в случае необходимости
    void checkAndRestoreSettings();

private:
    QList< SettingsData > listKeys; // список пар <ключ, заголовок, тип редактирования>
};

#endif // APPSETTINGS_H
