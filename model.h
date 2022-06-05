#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QColor>
#include <QSettings>
#include <QVector2D>
#include <QtMath>

#include <DriveMode.h>


typedef QList<QVector2D> ListVector;

class Model : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor colorStatusBar READ colorStatusBar WRITE setColorStatusBar NOTIFY colorStatusBarChanged)

public:
    explicit Model(QObject *parent = nullptr);

    Q_INVOKABLE void setDriveModeFromQML(QVariant mode);

    Q_INVOKABLE bool isVisibleButtonAB();
    Q_INVOKABLE bool isEnableButtonAB();

    const QColor &colorStatusBar() const;
    void setColorStatusBar(const QColor &newColorStatusBar);

signals:
    void signalSendToQML(QString str);

    void signalSetPointA();
    void signalSetPointB();

    void signalAppPointToPathQML(const QVector2D vec);// добавляем точку в траекторию
    void signalAppPointToPathAndRemoveFirstQML(const QVector2D vec);//добавляем и удаляем
    void keyPointsToQML(const ListVector keyPoints);
    void sendKeyPointForAdding(const QVector2D &vec);//посылаем ключевую точку в autopilot
    void sendDriveMode(const QVariant &mode);// посылаем режим вождения в autopilot

    void sendPointAToQML(const QVector2D point);
    void sendDirectToQML(const QVector2D dir);

    void signalCommandToSlave14(const int &comm);

    // эти сигналы можно переименовать, но они не для демонстрации
    void signalStateGPStoQML(bool state);

    void signalStateI2CtoQML(bool state);
    void signalStateGyrotoQML(bool state);
    void signalStateCamtoQML(bool state);
    //

    void colorStatusBarChanged();

    void signalRotateLeftToQML();///////////////////////////////////////////////////////
    void signalRotateRightToQML();///////////////////////////////////////////////////////
    void signalCountToQML(qreal count);

public slots:
    void slotGPSon();
    void slotGPSoff();

    // слоты для демонстрации
    void slotUpdateTimerI2C();
    void slotUpdateTimerGyro();
    void slotUpdateTimerCam();
    //

    void slotCommandToSlave14(int comm);

    void slotAppPointToPath(const QVector2D &vec);

    void slotAppPointToPathAndRemoveFirst(const QVector2D &vec);

    void acceptKeyPoints(const ListVector &keyPoints);

    void addKeyPointFromQML(const QVector2D point);

    void addPointAToQML(const QVector2D &point);
    void addDirectToQML(const QVector2D &dir);

    void slotRotateFromAutopilot(const int &msec);////////////////////////////////////////////////

private slots:
    void disableButtonAB();

private:
    QString testData;

    ListVector keyPoints;

    // таймеры и состояния для демонстрации
    QTimer *timerGPS;
    bool stateGPS = false;

    QTimer *timerI2C;
    bool stateI2C = false;

    QTimer *timerGyro;
    bool stateGyro = false;

    QTimer *timerCam;
    bool stateCam = false;
    //


    QColor m_colorStatusBar;

    bool isEnableAB = true;


    qreal count = 0;
};

#endif // MODEL_H
