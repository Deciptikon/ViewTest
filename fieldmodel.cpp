#include "fieldmodel.h"
#include "databasefield.h"

FieldModel::FieldModel(QObject *parent) : QSqlQueryModel(parent)
{
    this->updateModel();
}

QVariant FieldModel::data(const QModelIndex &index, int role) const
{
    // Определяем номер колонки по номеру роли
    int columnId = role - Qt::UserRole - 1;
    // Создаём индекс с помощью новоиспечённого ID колонки
    QModelIndex modelIndex = this->index(index.row(), columnId);

    /* И с помощью уже метода data() базового класса
     * вытаскиваем данные для таблицы из модели
     * */
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> FieldModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole]           = "id";
    roles[NameRole]         = "name";
    roles[SizeRole]         = "size";
    roles[PathToDataRole]   = "pathToData";
    return roles;
}

void FieldModel::updateModel()
{
    this->setQuery("SELECT id, " TABLE_NAME ", " TABLE_SIZE ", " TABLE_PATHTODATA " FROM " TABLE);
}

int FieldModel::getId(int row)
{
    return this->data(this->index(row, 0), IdRole).toInt();
}
