#include "appsettings.h"

AppSettings::AppSettings(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() << "регистрация приложения";
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);


    // формирования списка настроек
    SettingsData data;

    // <key, title, typeEdit>
    // key      - ключ для записи/чтения настроек
    // title    - заголовок для делегата view
    // typeEdit - тип редактирования значений настроек по ключу key
    // QObject::tr() - пометка для автоматического переводчика
    data = {EMPTY_STRING,
            QObject::tr("Общие настройки приложения"),
            TypeEdit::NONE_EDIT };
    listKeys.append(data);

    data = {"widthWindow", /// эти ключи только для демонстрации и потом не понадобятся
            QObject::tr("Width basic window(test)"),
            TypeEdit::DOUBLE_EDIT };
    listKeys.append(data);

    data = {"heightWindow", /// демо...
            QObject::tr("Height basic window(test)"),
            TypeEdit::DOUBLE_EDIT };
    listKeys.append(data);

    data = {DIR_STATUSBAR KEY_COLOR_STATUSBAR, // а это будет нужно
            QObject::tr("Color status bar"),
            TypeEdit::STRING_EDIT };
    listKeys.append(data);

    data = {EMPTY_STRING, // Заголовок для группы настроек, не содержит ключ/значение
            QObject::tr("Настройки сохранения траектории"),
            TypeEdit::NONE_EDIT };
    listKeys.append(data);

    data = {"flagSaveTraectory", /// демо...
            QObject::tr("Сохранять траекторию на устройстве?"),
            TypeEdit::BOOL_EDIT };
    listKeys.append(data);

    data = {"flagUpdateGPS", /// демо...
            QObject::tr("Включить ориентацию по GPS?"),
            TypeEdit::BOOL_EDIT };
    listKeys.append(data);
}

int AppSettings::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return listKeys.size();
}

QVariant AppSettings::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case TitleRole: {
        return listKeys.at(index.row()).title;// заголовок из пары <ключ, заголовок>
    }
    case ValueRole: {
        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
        QString key = listKeys.at(index.row()).key;
        // значение по ключу из <ключ, заголовок, тип редактирования>
        QVariant var = settings.value(key);
        qDebug() << "getData(index =" << index.row() << ", role =" << role <<" ) = " << var;
        if(listKeys.at(index.row()).typeEdit == TypeEdit::BOOL_EDIT) {
            int q = var.toBool() ? 1 : 0;// qml приведёт 0 к false
            return QVariant(q);
        }
        return var;
    }
    case TypeEditRole: {
        return listKeys.at(index.row()).typeEdit;
    }
    default: {
        return QVariant();
    }
    }
}

QHash<int, QByteArray> AppSettings::roleNames() const
{
    // связь ролей с свойствами в qml
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[TitleRole]    = "title";
    roles[ValueRole]    = "value";
    roles[TypeEditRole] = "typeEdit";

    return roles;
}

bool AppSettings::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }
    if(role != ValueRole) {
        return false;
    }

    qDebug() << "setData() value:" << value;
    QString key = listKeys.at(index.row()).key;// ключ по индексу
    TypeEdit::State type = listKeys.at(index.row()).typeEdit;//тип

    if(type == TypeEdit::NONE_EDIT) {
        return false;
    }

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue( key, value);// устанавливаем значение по ключу
    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) { // если ошибок нет, возвращаем true
        qDebug() << "QSettings::NoError";
        qDebug() << " ";
        // испускаем сигнал о изменившихся данных для обновления view
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    } else {
        qDebug() << "QSettings::Error";
        // здесь можно излучить сигнал с ошибкой и отобразить на view
        return false;
    }
}

Qt::ItemFlags AppSettings::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant AppSettings::getValue(const QString key)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    return settings.value(key);
}

QString AppSettings::getValueString(const QString key)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    return settings.value(key, "").toString();
}

qreal AppSettings::getValueQreal(const QString key)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    return settings.value(key, 0).toReal();
}

int AppSettings::getValueInt(const QString key)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    return settings.value(key, 0).toInt();
}

bool AppSettings::getValueBool(const QString key)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    return settings.value(key, false).toBool();
}

void AppSettings::setValue(QString key, QVariant value)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(key, value);
    settings.sync();
    if(settings.status() == QSettings::NoError) {
        // это нужно поправить)))) сигнал должен излучаться что бы view отражала изменения
        //emit dataChanged(index, index, QVector<int>() << Roles::ValueRole);
    }
}
