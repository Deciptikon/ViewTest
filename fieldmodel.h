#ifndef FIELDMODEL_H
#define FIELDMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class FieldModel : public QSqlQueryModel
{
    Q_OBJECT
public:

    enum Roles {
        IdRole = Qt::UserRole + 1,     // id, всё остальное обязательно!!! ниже
        /// Наименование поля.
        NameRole,
        /// Размер поля, в м**2.
        SizeRole,
        /// Путь к файлу(или имя файла) с данными трактории данного поля.
        PathToDataRole
    };

    explicit FieldModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

signals:


public slots:
    /// Слот обновления модели.
    void updateModel();
    int getId(int row);

};

#endif // FIELDMODEL_H
