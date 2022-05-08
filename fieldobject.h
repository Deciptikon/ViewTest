#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include <QObject>

class FieldObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)

public:
    explicit FieldObject(QObject *parent = nullptr);
    FieldObject(QString name, QString size);

    const QString &name() const;
    void setName(const QString &newName);

    const QString &size() const;
    void setSize(const QString &newSize);

signals:

    void nameChanged();

    void sizeChanged();

private:
    QString m_name;
    QString m_size;
};

#endif // FIELDOBJECT_H
