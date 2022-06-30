#ifndef DATABASEFIELD_H
#define DATABASEFIELD_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDebug>

//#include "constants.h"


#define DATABASE_HOSTNAME   "FieldDataBase"
#define DATABASE_NAME       "Fields.db"
#define DATABASE_PATH       "C:/"
#define DATABASE_FOLDER     "fieldsdb/"

#define TABLE               "FieldsTable"   // Название таблицы
#define TABLE_NAME          "Name"          // Вторая колонка
#define TABLE_SIZE          "Size"          // Третья колонка
#define TABLE_PATHTODATA    "PathToData"    // Четвертая колонка

class DataBaseField : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseField(QObject *parent = nullptr);

    void connectToDataBase();

signals:
    //

public slots:
    /// Добавление записей в таблицу базы данных.
    bool inserIntoTable(const QVariantList &data);

    /// Добавление записей в таблицу базы данных.
    bool inserIntoTable(const QString &fname, const QString &sname, const QString &nik);

    /// Удаление записи из таблицы по её id
    bool removeRecord(const int id);

private:
    /// Открытие базы данных.
    bool openDataBase();

    /// Восстановление базы данных.
    bool restoreDataBase();

    /// Закрытие базы данных.
    void closeDataBase();

    /// Создание базы таблицы в базе данных
    bool createTable();

    QSqlDatabase db;

};

#endif // DATABASEFIELD_H
