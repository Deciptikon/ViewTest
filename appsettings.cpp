#include "appsettings.h"

AppSettings::AppSettings(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() << "регистрация приложения";
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    qDebug() << "получение настроек";
    //QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    QPair<QString, QString> data1{"widthWindow", "Width general window"};
    listKeys.append(data1);

    QPair<QString, QString> data2{"heightWindow", "Height general window"};
    listKeys.append(data2);

    QPair<QString, QString> data3{"colorWindow", "Color general window"};
    listKeys.append(data3);
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
    qDebug() << "AppSettings::data(const QModelIndex &index, int role) const";
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case TitleRole: {
        //qDebug() << "case TitleRole";
        QString r = listKeys.at(index.row()).second;
        qDebug() << "case TitleRole; " << r;
        return r;// заголовок из пары <ключ, заголовок>
    }
    case ValueRole: {
        //qDebug() << "case ValueRole";
        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
        QVariant r = settings.value(listKeys.at(index.row()).first, "");
        qDebug() << "case ValueRole: " << r.toString();
        return r;// значение по ключу из пары <ключ, заголовок>
    }
    default: {
        return QVariant();
    }
    }
}

QHash<int, QByteArray> AppSettings::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[TitleRole] = "title";
    roles[ValueRole] = "value";

    return roles;
}

bool AppSettings::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //qDebug() << "setData()";
    if (!index.isValid()) {
        return false;
    }

    switch (role) {
    case TitleRole: {
        return false;   // Не меняем title из view
    }
    case ValueRole: {
        qDebug() << "setData() case ValueRole:" << value;

        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

        QString key = listKeys.at(index.row()).first;// ключ по индексу
        settings.setValue( key, value.toString());// устанавливаем значение по ключу
        settings.sync(); // синхронизируемся и получаем статус

        if(settings.status() == QSettings::NoError) { // если ошибок нет, возвращаем true
            //emit settingsChanged(); // излучаем сигнал о изменении
            //return true;
            qDebug() << "settings.status() == QSettings::NoError";
        } else {
            qDebug() << "settings.status() != QSettings::NoError";
            return false;
        }
    }
    default: {
        return false;
    }
    }

    emit dataChanged(index, index, QVector<int>() << role);

    return true;
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
        //emit settingsChanged();

        //emit dataChanged(index, index, QVector<int>() << Roles::ValueRole);
    }
}
