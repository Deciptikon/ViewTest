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
    bool inserIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool inserIntoTable(const QString &fname, const QString &sname, const QString &nik);
    bool removeRecord(const int id); // Удаление записи из таблицы по её id

private:
    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание базы таблицы в базе данных

    QSqlDatabase db;

};

#endif // DATABASEFIELD_H
