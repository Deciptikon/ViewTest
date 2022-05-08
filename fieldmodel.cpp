#include "fieldmodel.h"

FieldModel::FieldModel(QObject *parent) : QObject(parent)
{
    listField.append(new FieldObject("field model 1", "12345"));
    listField.append(new FieldObject("field model 2", "22222"));
    listField.append(new FieldObject("field model 33", "54321"));
    listField.append(new FieldObject("field model 45", "45546"));
}

const QList<QObject *> &FieldModel::getListField() const
{
    return listField;
}

void FieldModel::setListField(const QList<QObject *> &newListField)
{
    listField = newListField;
}
