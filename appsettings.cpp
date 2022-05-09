#include "appsettings.h"

AppSettings::AppSettings(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() << "регистрация приложения";
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    qDebug() << "получение настроек";

    //QPair<QString, QString> data;
    SettingsData data;


    // <key, title, typeEdit> (ключ для записи/чтения настроек и заголовок для view)
    data = {"", "Общие настройки окна", TypeEdit::NONE_EDIT};
    listKeys.append(data);

    data = {"widthWindow", "Width general window", TypeEdit::DOUBLE_EDIT};
    listKeys.append(data);

    data = {"heightWindow", "Height general window", TypeEdit::DOUBLE_EDIT};
    listKeys.append(data);

    data = {"colorWindow", "Color general window", TypeEdit::STRING_EDIT};
    listKeys.append(data);

    data = {"", "Настройки сохранения траектрии", TypeEdit::NONE_EDIT};
    listKeys.append(data);

    data = {"flagSaveTraectory", "Сохранять траекторию на устройстве?", TypeEdit::BOOL_EDIT};
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
    qDebug() << "AppSettings::data(index =" << index << ", role =" << role <<" ) const";
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
        TypeEdit::State type = listKeys.at(index.row()).typeEdit;
        // значение по ключу из пары <ключ, заголовок, тип редактирования>
        if(type == TypeEdit::NONE_EDIT) {
            qDebug() << "none";
            return QVariant();
        }
        if(type == TypeEdit::BOOL_EDIT) {
            bool val = settings.value(key, false).Bool;
            qDebug() << val;
            return QVariant(val);
        }
        if(type == TypeEdit::DOUBLE_EDIT) {
            double val = settings.value(key, 0).Double;
            qDebug() << val;
            return settings.value(key, 0).Double;
        }
        if(type == TypeEdit::STRING_EDIT) {
            QString val(settings.value(key, "").String);
            qDebug() << val;
            return QVariant(val);
        }

    }
    case IsNoneEditRole: {
        return listKeys.at(index.row()).typeEdit == TypeEdit::NONE_EDIT ? true: false;
    }
    case IsBoolEditRole: {
        return listKeys.at(index.row()).typeEdit == TypeEdit::BOOL_EDIT ? true: false;
    }
    case IsDoubleEditRole: {
        return listKeys.at(index.row()).typeEdit == TypeEdit::DOUBLE_EDIT ? true: false;
    }
    case IsStringEditRole: {
        return listKeys.at(index.row()).typeEdit == TypeEdit::STRING_EDIT ? true: false;
    }
    default: {
        return QVariant();
    }
    }
}

QHash<int, QByteArray> AppSettings::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[TitleRole]        = "title";
    roles[ValueRole]        = "value";
    roles[IsNoneEditRole]   = "isNoneEdit";
    roles[IsBoolEditRole]   = "isBoolEdit";
    roles[IsDoubleEditRole] = "isDoubleEdit";
    roles[IsStringEditRole] = "isStringEdit";

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

    qDebug() << "setData() case ValueRole:" << value;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    QString key = listKeys.at(index.row()).key;// ключ по индексу
    settings.setValue( key, value.toString());// устанавливаем значение по ключу
    settings.sync(); // синхронизируемся и получаем статус

    if(settings.status() == QSettings::NoError) { // если ошибок нет, возвращаем true
        qDebug() << "QSettings::NoError";
        // испускаем сигнал о изменившихся данных для обновления view
        emit dataChanged(index, index, QVector<int>() << role);
        qDebug() << "dataChanged(index, index, QVector<int>() << role)";
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

//void AppSettings::add()
//{
//    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
//    m_data.append("new");
//    endInsertRows();

//    m_data[0] = QString("Size: %1").arg(m_data.size());
//    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
//    emit dataChanged(index, index);
//}

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
