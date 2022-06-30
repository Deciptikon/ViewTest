#ifndef LOCALBASIS_H
#define LOCALBASIS_H

#include <QVector3D>
#include <QDebug>

#include "constants.h"

class LocalBasis
{
public:
    LocalBasis();

    void debug();

    bool saveBasis();

    bool readBasis();

    void setLocalX(const QVector3D &vec);
    const QVector3D &localX() const;

    const QVector3D &localY() const;

    void setLocalZ(const QVector3D &vec);
    const QVector3D &localZ() const;

    QVector3D toLocalBasis(const QVector3D &vec);

private:
    QVector3D m_localX;
    QVector3D m_localY;
    QVector3D m_localZ;
};

#endif // LOCALBASIS_H
