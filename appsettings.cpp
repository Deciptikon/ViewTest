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
    // defaultValue - значение по умолчанию (нужно для инициации и сброса настроек)
    // QObject::tr() - пометка для автоматического переводчика
    data = {EMPTY_STRING,
            QObject::tr("Basic settings"),
            TypeEdit::NONE_EDIT,
            ""};
    listKeys.append(data);

    data = {"widthWindow", /// эти ключи только для демонстрации и потом не понадобятся
            QObject::tr("Width basic window(test)"),
            TypeEdit::DOUBLE_EDIT,
            "800"};
    listKeys.append(data);

    data = {"heightWindow", /// демо...
            QObject::tr("Height basic window(test)"),
            TypeEdit::DOUBLE_EDIT,
            "600"};
    listKeys.append(data);


    /* -----------------------
     * Настройки внешнего вида
     * */
    data = {DIR_STATUSBAR KEY_COLOR_STATUSBAR, // это будет нужно
            QObject::tr("Color status bar"),
            TypeEdit::STRING_EDIT,
            DEFAULT_COLOR_STATUSBAR};
    listKeys.append(data);

    data = {EMPTY_STRING, // Заголовок для группы настроек, не содержит ключ/значение
            QObject::tr("Trajectory settings"),
            TypeEdit::NONE_EDIT,
            ""};
    listKeys.append(data);

    data = {"flagSaveTraectory", /// демо...
            QObject::tr("Save trajectory on device?"),
            TypeEdit::BOOL_EDIT,
            "false"};
    listKeys.append(data);

    data = {"flagUpdateGPS", /// демо...
            QObject::tr("On GPS?"),
            TypeEdit::BOOL_EDIT,
            "false"};
    listKeys.append(data);


    /* ---------------
     * Настройки языка
     * */
    data = {EMPTY_STRING, // Заголовок для группы настроек, не содержит ключ/значение
            QObject::tr("Language settings"),
            TypeEdit::NONE_EDIT,
            ""};
    listKeys.append(data);

    // текущий язык: "_ru", "_ru_RU", "_en", "_en_US", "_en_GB", ...
    data = {DIR_LANGUAGE KEY_CURRENT_LANGUAGE,
            QObject::tr("Current language"),
            TypeEdit::STRING_EDIT, /// изменить на выпадающий список...
            DEFAULT_CURRENT_LANGUAGE};
    listKeys.append(data);







    /* ============================================================================================
     * Сброс настроек(пусть будет расположен в конце)
     * */
    data = {EMPTY_STRING, // Заголовок для группы настроек, не содержит ключ/значение
            QObject::tr("Return to factory settings"),
            TypeEdit::NONE_EDIT,
            ""};
    listKeys.append(data);

    // Востановление настроек
    data = {DIR_RESTORE_SETTINGS KEY_RESTORE_SETTINGS,
            QObject::tr("Restore settings?(after reboot)"),
            TypeEdit::BOOL_EDIT,
            DEFAULT_RESTORE_SETTINGS};
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

        SettingsData data = listKeys.at(index.row());

        // значение по ключу из <ключ, заголовок, тип редактирования, ...>
        QVariant var = settings.value(data.key, data.defaultValue);
        qDebug() << "getData(index =" << index.row() << ", role =" << role <<" ) = " << var;

        if(data.typeEdit == TypeEdit::BOOL_EDIT) {
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

void AppSettings::checkAndRestoreSettings()
{
    if(listKeys.isEmpty()) {
        return;
    }

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    QVariant var = settings.value(DIR_RESTORE_SETTINGS KEY_RESTORE_SETTINGS,
                                  DEFAULT_RESTORE_SETTINGS );
    if(var.toBool()) {
        for(auto data: listKeys) {
            settings.setValue(data.key, data.defaultValue);
        }
    }
//    if(var.toBool()) {
//        qDebug() << "Востановление заводских настроек";
//        settings.setValue(DIR_STATUSBAR KEY_COLOR_STATUSBAR, DEFAULT_COLOR_STATUSBAR);
//        settings.setValue(DIR_LANGUAGE KEY_CURRENT_LANGUAGE, DEFAULT_CURRENT_LANGUAGE);


//        // в конце устанавливаем востановление заводских настроек на "false"
//        settings.setValue(DIR_RESTORE_SETTINGS KEY_RESTORE_SETTINGS, DEFAULT_RESTORE_SETTINGS);
//        qDebug() << "Востановление настроек завершено";
//    }
}

