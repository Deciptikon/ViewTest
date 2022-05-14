#ifndef DRIVEMODE_H
#define DRIVEMODE_H

#include <QObject>

// ENUM для использования как в с++ так и в qml
class DriveMode : public QObject
{
    Q_OBJECT
public:
    enum State {
        NONE_MODE,      // нет режима
        KEYPOINTS_MODE, // движение по ключевым точкам
        PARALLEL_MODE,  // параллельное вождение
        SPIRAL_MODE     // по спирали (вдоль замкнутой кривой, вдоль старой кромки)
    };
    Q_ENUM(State)
};

#endif // DRIVEMODE_H
