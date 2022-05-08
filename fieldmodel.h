#ifndef FIELDMODEL_H
#define FIELDMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class FieldModel : public QSqlQueryModel
{
    Q_OBJECT
public:

    enum Roles {
        IdRole = Qt::UserRole + 1,      // id
        FNameRole,                      // имя
        SNameRole,                      // фамилия
        NikRole                         // ник
    };

    explicit FieldModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    /* хешированная таблица ролей для колонок.
     * Метод используется в дебрях базового класса QAbstractItemModel,
     * от которого наследован класс QSqlQueryModel
     * */
    QHash<int, QByteArray> roleNames() const;

signals:


public slots:
    void updateModel();
    int getId(int row);

};

#endif // FIELDMODEL_H
