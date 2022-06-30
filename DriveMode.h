#ifndef DRIVEMODE_H
#define DRIVEMODE_H

#include <QObject>

// ENUM для использования как в с++ так и в qml
class DriveMode : public QObject
{
    Q_OBJECT
public:
    enum State {
        /// Нет режима.
        NONE_MODE,
        /// Режим движениея по ключевым точкам.
        KEYPOINTS_MODE,
        /// Режим параллельного вождения.
        PARALLEL_MODE,
        /// Режим движения по спирали (вдоль замкнутой кривой, вдоль старой кромки).
        SPIRAL_MODE
    };
    Q_ENUM(State)
};

#endif // DRIVEMODE_H
