#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QColor>
#include <QSettings>
#include <QVector2D>
#include <QVector3D>
#include <QtMath>

#include <DriveMode.h>


typedef QList<QVector2D> ListVector;

class Model : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor colorStatusBar READ colorStatusBar WRITE setColorStatusBar NOTIFY colorStatusBarChanged)

public:
    explicit Model(QObject *parent = nullptr);

    /// Устанавливает режим вождения полученный из QML,
    /// переключает некоторые флаги.
    Q_INVOKABLE void setDriveModeFromQML(QVariant mode);

    /// Возвращает значение флага видимости кнопки установки точек А и В.
    Q_INVOKABLE bool isVisibleButtonAB();
    /// Возвращает значение флага активности кнопки установки точек А и В.
    Q_INVOKABLE bool isEnableButtonAB();

    /// Отправляет данные на подруливающее устройство из слоя QML.
    Q_INVOKABLE void sendToDevice14(qreal data);
    /// Переключает состояние реле (отправляя соответствующую команду
    /// на подруливающее устройство).
    Q_INVOKABLE void swapRelayState();

    /// Возвращает значение "true" если выполняется на ситеме Linux.
    Q_INVOKABLE bool isLinux();

    /// Возвращает расстояние между параллельными линиями
    /// прочитанное из настроек.
    Q_INVOKABLE qreal getWidthBetweenLines();

    /// Устанавливает новое расстояние между параллельными линиями
    /// в режиме параллельного вождения.
    Q_INVOKABLE void setWidthBetweenLines(qreal width);

    /// Возвращает цвет статусбара.
    const QColor &colorStatusBar() const;
    /// Устанавливает цвет статусбара.
    void setColorStatusBar(const QColor &newColorStatusBar);

signals:
    /// Видимо это не нужно ....
    void signalSendToQML(QString str);

    /// Сигнал о установки точки А вызываемый из QML и передаваемый в
    /// автопилот.
    void signalSetPointA();
    /// Сигнал о установки точки В вызываемый из QML и передаваемый в
    /// автопилот.
    void signalSetPointB();

    /// Сигнал добавления точки, полученной от автопилота, в QML.
    void signalAppPointToPathQML(const QVector2D vec);
    /// Сигнал добавления точки, полученной от автопилота, в QML и
    /// удаления первой точки.
    void signalAppPointToPathAndRemoveFirstQML(const QVector2D vec);
    /// Сигнал добавления ключевой точки, полученной от автопилота, в QML.
    void keyPointsToQML(const ListVector keyPoints);
    /// Сигнал отправки ключевой точки из QML в автопилот.
    void sendKeyPointForAdding(const QVector2D &vec);
    /// Сигнал отправки режима вождения в автопилот.
    void sendDriveMode(const QVariant &mode);

    /// Передача точки А в слой QML.
    void sendPointAToQML(const QVector2D point);
    /// Передача направления линий параллельного вождения в слой QML.
    void sendDirectToQML(const QVector2D dir);

    /// Сигнал отправки команды на подруливающее устройство.
    void signalCommandToSlave14(const int &comm);

    /// Передача данных гироскопа в слой QML.
    void signalDataGyrosToQML(qreal x, qreal y, qreal z);
    /// Передача данных акселерометра в слой QML.
    void signalDataAccelToQML(qreal x, qreal y, qreal z);


    /// Сигнал старта колибровки покоя акселерометра с длительностью
    /// калибровки в миллисекундах.
    void signalCalibrateZeroPointAccelerometer(const int &msec);
    /// Сигнал старта колибровки покоя акселерометра с длительностью
    /// калибровки в миллисекундах.
    void signalCalibrateZeroPointGyroscope(const int &msec);
    /// Сигнал успешного прохождения калибровки акселерометра
    /// полученый от системы.
    void signalCalibrateZeroPointAccelerometerIsDone();
    /// Сигнал успешного прохождения калибровки гироскопа
    /// полученый от системы.
    void signalCalibrateZeroPointGyroscopeIsDone();

    /// Сигнал старта колибровки оси Z гироскопа.
    void signalCalibrateZAxisGyroscope();
    /// Сигнал успешного прохождения калибровки оси Z гироскопа
    /// полученый от системы.
    void signalCalibrateZAxisGyroscopeIsDone();

    /// Сигнал старта колибровки оси X акселерометра.
    void signalCalibrateXAxisAccelerometer();
    /// Сигнал успешного прохождения калибровки оси Х акселерометра
    /// полученый от системы.
    void signalCalibrateXAxisAccelerometerIsDone();


    // эти сигналы можно переименовать, но они не для демонстрации
    /// Сигнал состояния активности модуля GPS, для отображения в
    /// статусбаре QML.
    void signalStateGPStoQML(bool state);
    /// Сигнал состояния активности модуля I2C, для отображения в
    /// статусбаре QML.
    void signalStateI2CtoQML(bool state);
    /// Сигнал состояния активности модуля гироскопа и акселерометра
    /// , для отображения в статусбаре QML.
    void signalStateGyrotoQML(bool state);
    /// Сигнал состояния активности модуля цифровой камеры, для отображения в
    /// статусбаре QML.
    void signalStateCamtoQML(bool state);
    //---------

    /// Сигнал о изменении цвета статусбара.
    void colorStatusBarChanged();

    /// Сигнал устанавливающий расстояние между параллельными линиями
    /// в режиме параллельного вождения.
    void signalUpdateWidthBetweenLines(const float &width);

public slots:
    /// Слот приёма состояния активности модуля GPS.
    void slotGPSon();
    /// Слот приёма состояния не активности модуля GPS.
    void slotGPSoff();

    // слоты для демонстрации
    void slotUpdateTimerI2C();
    void slotUpdateTimerGyro();
    void slotUpdateTimerCam();
    //

    /// Слот, принимает точку пути от автопилота для отображения в QML.
    void slotAppPointToPath(const QVector2D &vec);
    /// Слот, принимает точку пути (и удаляет первую) от автопилота для отображения в QML.
    void slotAppPointToPathAndRemoveFirst(const QVector2D &vec);

    /// Слот, принимает от автопилота список ключевых точек (для передачи их в QML).
    void acceptKeyPoints(const ListVector &keyPoints);

    /// Слот, отправляет ключевую точку пути из QML в автопилот.
    void addKeyPointFromQML(const QVector2D point);
    /// Слот, принимает текущие данные акселерометра и гироскопа от системы.
    void slotDataSensToQML(const QVector3D &accel, const QVector3D &gyro);

    /// Слот, принимает точку А от автопилота для отображении её в QML.
    void addPointAToQML(const QVector2D &point);
    /// Слот, принимает направление параллельных линий от автопилота
    /// для отображении их в QML.
    void addDirectToQML(const QVector2D &dir);

    /// Слот приема успешности калибровки состояния покоя акселерометра.
    void slotCalibrateZeroPointAccelerometerIsDone();
    /// Слот приема успешности калибровки состояния покоя гироскопа.
    void slotCalibrateZeroPointGyroscopeIsDone();

    /// Слот приема успешности калибровки оси Z гироскопа.
    void slotCalibrateZAxisGyroscopeIsDone();
    /// Слот приема успешности калибровки оси Z акселерометра.
    void slotCalibrateXAxisAccelerometerIsDone();

private slots:
    /// Слот отключения активности кнопки выбора точек А и В.
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
