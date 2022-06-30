#ifndef LOCALBASIS_H
#define LOCALBASIS_H

#include <QVector3D>
#include <QDebug>

#include "constants.h"

class LocalBasis
{
public:
    LocalBasis();

    /// Вывод текущего сосотояния через qDebug().
    void debug();

    /// Сохраняет три текущих базисных вектора в настройки приложения.
    bool saveBasis();

    /// Читает три базисных вектора из настроек приложения.
    bool readBasis();

    /// Устанавливает новый вектор в качестве оси Х локального базиса.
    void setLocalX(const QVector3D &vec);
    /// Возвращает вектор текущей оси Х локального базиса.
    const QVector3D &localX() const;

    /// Возвращает вектор текущей оси Y локального базиса.
    const QVector3D &localY() const;

    /// Устанавливает новый вектор в качестве оси Z локального базиса.
    void setLocalZ(const QVector3D &vec);
    /// Возвращает вектор текущей оси Z локального базиса.
    const QVector3D &localZ() const;

    /// Возвращает вектор в координатах локального базиса полученный
    /// из вектора в общем базисе.
    QVector3D toLocalBasis(const QVector3D &vec);

private:
    QVector3D m_localX;
    QVector3D m_localY;
    QVector3D m_localZ;
};

#endif // LOCALBASIS_H
