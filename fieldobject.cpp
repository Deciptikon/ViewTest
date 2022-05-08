#include "fieldobject.h"

FieldObject::FieldObject(QObject *parent) : QObject(parent)
{

}

FieldObject::FieldObject(QString name, QString size) {
    setName(name);
    setSize(size);
}

const QString &FieldObject::name() const
{
    return m_name;
}

void FieldObject::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

const QString &FieldObject::size() const
{
    return m_size;
}

void FieldObject::setSize(const QString &newSize)
{
    if (m_size == newSize)
        return;
    m_size = newSize;
    emit sizeChanged();
}
