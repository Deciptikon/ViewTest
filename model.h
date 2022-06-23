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

    Q_INVOKABLE void sendToDevice14(qreal data);
    Q_INVOKABLE void swapRelayState();

    Q_INVOKABLE bool isLinux();

    const QColor &colorStatusBar() const;
    void setColorStatusBar(const QColor &newColorStatusBar);

signals:
    void signalSendToQML(QString str);

    // установить точки А и В из QML
    void signalSetPointA();
    void signalSetPointB();

    void signalAppPointToPathQML(const QVector2D vec);// добавляем точку в траекторию
    void signalAppPointToPathAndRemoveFirstQML(const QVector2D vec);//добавляем и удаляем
    void keyPointsToQML(const ListVector keyPoints);
    void sendKeyPointForAdding(const QVector2D &vec);//посылаем ключевую точку в autopilot
    void sendDriveMode(const QVariant &mode);// посылаем режим вождения в autopilot

    // отправка точки А и направления в слой QML
    void sendPointAToQML(const QVector2D point);
    void sendDirectToQML(const QVector2D dir);

    // отправить команду на устройство
    void signalCommandToSlave14(const int &comm);

    // сигнал старта колибровки покоя акселерометра и гироскопа
    void signalCalibrateZeroPointAccelerometer(const int &msec);
    void signalCalibrateZeroPointGyroscope(const int &msec);
    // сигнал успешной калибровки полученый от системы
    void signalCalibrateZeroPointAccelerometerIsDone();
    void signalCalibrateZeroPointGyroscopeIsDone();

    // эти сигналы можно переименовать, но они не для демонстрации
    void signalStateGPStoQML(bool state);

    void signalStateI2CtoQML(bool state);
    void signalStateGyrotoQML(bool state);
    void signalStateCamtoQML(bool state);
    //

    void colorStatusBarChanged();

public slots:
    void slotGPSon();
    void slotGPSoff();

    // слоты для демонстрации
    void slotUpdateTimerI2C();
    void slotUpdateTimerGyro();
    void slotUpdateTimerCam();
    //

    // принимаем точку пути от автопилота для отображения в QML
    void slotAppPointToPath(const QVector2D &vec);

    void slotAppPointToPathAndRemoveFirst(const QVector2D &vec);

    // принимаем от автопилота список ключевых точек (для передачи их в QML)
    void acceptKeyPoints(const ListVector &keyPoints);

    // отправить ключевую точку пути из QML
    void addKeyPointFromQML(const QVector2D point);

    // слоты приема данных от автопилота для передачи их в QML
    void addPointAToQML(const QVector2D &point);
    void addDirectToQML(const QVector2D &dir);

    // слот приема успешности калибровки состояния покоя
    // акселерометра и гироскопа
    void slotCalibrateZeroPointAccelerometerIsDone();
    void slotCalibrateZeroPointGyroscopeIsDone();

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

    bool osLinux = false;
};

#endif // MODEL_H
