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
        NameRole,                      // Имя
        SizeRole,                      // Размер, в м**2
        PathToDataRole                 // Путь к файлу(или имя файла) с данными трактории и тд
    };

    explicit FieldModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

signals:


public slots:
    void updateModel();
    int getId(int row);

};

#endif // FIELDMODEL_H
