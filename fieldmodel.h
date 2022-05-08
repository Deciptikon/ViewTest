#ifndef FIELDMODEL_H
#define FIELDMODEL_H

#include <QObject>
#include <QDebug>

#include "fieldobject.h"

class FieldModel : public QObject
{
    Q_OBJECT
public:
    explicit FieldModel(QObject *parent = nullptr);

    const QList<QObject *> &getListField() const;
    void setListField(const QList<QObject *> &newListField);

signals:
    //

public slots:
    //

private:
    QList<QObject*> listField;

};

#endif // FIELDMODEL_H
