#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QColor>
#include <QSettings>


class Model : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor colorStatusBar READ colorStatusBar WRITE setColorStatusBar NOTIFY colorStatusBarChanged)

public:
    explicit Model(QObject *parent = nullptr);

    Q_INVOKABLE void testMetod(QString str);


    const QColor &colorStatusBar() const;
    void setColorStatusBar(const QColor &newColorStatusBar);

signals:
    void signalSendToQML(QString str);

    // эти сигналы можно переименовать, но они не для демонстрации
    void signalStateGPStoQML(bool state);
    void signalStateI2CtoQML(bool state);
    void signalStateGyrotoQML(bool state);
    void signalStateCamtoQML(bool state);
    //

    void colorStatusBarChanged();

public slots:
    void slotTakeFromQML(QString str);

    // слоты для демонстрации
    void slotUpdateTimerGPS();
    void slotUpdateTimerI2C();
    void slotUpdateTimerGyro();
    void slotUpdateTimerCam();
    //


private:
    QString testData;

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
};

#endif // MODEL_H
