#ifndef DATABASEFIELD_H
#define DATABASEFIELD_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>


#define DATABASE_HOSTNAME   "NameDataBase"
#define DATABASE_NAME       "Name.db"

#define TABLE               "NameTable"         // Название таблицы
#define TABLE_FNAME         "FisrtName"         // Вторая колонка
#define TABLE_SNAME         "SurName"           // Третья колонка
#define TABLE_NIK           "Nik"               // Четвертая колонка

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
