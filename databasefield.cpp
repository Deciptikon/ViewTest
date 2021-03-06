#include "databasefield.h"

DataBaseField::DataBaseField(QObject *parent) : QObject(parent)
{

}

void DataBaseField::connectToDataBase()
{
    // проверяем существует ли директория, и создаем по необходимости (локально)
    if(!QDir(DATABASE_FOLDER).exists()) {//DATABASE_PATH DATABASE_FOLDER
        QString fld = DATABASE_FOLDER;
        QDir().mkdir(fld.remove(fld.length() - 1, 1));
    }
    // Перед подключением к базе данных производим проверку на её существование.
    // В зависимости от результата производим открытие базы данных или её восстановление
    if(QFile(DATABASE_FOLDER DATABASE_NAME).exists()){//DATABASE_PATH
        this->openDataBase();
    } else {
        this->restoreDataBase();
    }
}

bool DataBaseField::inserIntoTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " TABLE " ( " TABLE_NAME ", "
                                             TABLE_SIZE ", "
                                             TABLE_PATHTODATA " ) "
                     "VALUES (:Name, :Size, :PathToData)");
    query.bindValue(":Name",       data[0].toString());
    query.bindValue(":Size",       data[1].toString());
    query.bindValue(":PathToData",         data[2].toString());

    // Запрос методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBaseField::inserIntoTable(const QString &name, const QString &size, const QString &path)
{
    QVariantList data;
    data.append(name);
    data.append(size);
    data.append(path);

    if(inserIntoTable(data))
        return true;
    else
        return false;
}

bool DataBaseField::removeRecord(const int id)
{
    // Удаление строки из базы данных будет производитсья с помощью SQL-запроса
    QSqlQuery query;

    // Удаление производим по id записи, который передается в качестве аргумента функции
    query.prepare("DELETE FROM " TABLE " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    // Выполняем удаление
    if(!query.exec()){
        qDebug() << "error delete row " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBaseField::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName( DATABASE_FOLDER DATABASE_NAME);//DATABASE_PATH
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

bool DataBaseField::restoreDataBase()
{
    // Если база данных открылась ...
    if(this->openDataBase()){
        // Производим восстановление базы данных
        return (this->createTable()) ? true : false;
        //return this->createTable();
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

void DataBaseField::closeDataBase()
{
    db.close();
}

bool DataBaseField::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    TABLE_NAME       " VARCHAR(255)    NOT NULL,"
                    TABLE_SIZE       " VARCHAR(255)    NOT NULL,"
                    TABLE_PATHTODATA " VARCHAR(255)    NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
