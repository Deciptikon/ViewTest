#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QColor>
#include <QSettings>
#include <QVector2D>

#include <DriveMode.h>


class Model : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor colorStatusBar READ colorStatusBar WRITE setColorStatusBar NOTIFY colorStatusBarChanged)

public:
    explicit Model(QObject *parent = nullptr);

    Q_INVOKABLE void testMetod(QString str);
    Q_INVOKABLE void setDriveModeFromQML(QVariant mode);

    const QColor &colorStatusBar() const;
    void setColorStatusBar(const QColor &newColorStatusBar);

signals:
    void signalSendToQML(QString str);

    void signalAppPointToPathQML(const QVector2D vec);// добавляем точку в траекторию
    void signalAppPointToPathAndRemoveFirstQML(const QVector2D vec);//добавляем и удаляем

    // эти сигналы можно переименовать, но они не для демонстрации
    void signalStateGPStoQML(bool state);

    void signalStateI2CtoQML(bool state);
    void signalStateGyrotoQML(bool state);
    void signalStateCamtoQML(bool state);
    //

    void colorStatusBarChanged();

public slots:
    void slotTakeFromQML(QString str);

    void acceptCoordXY(const double& x, const double& y);//получаем данные с GPS

    void slotGPSon();
    void slotGPSoff();

    // слоты для демонстрации
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




    double xorig=0, yorig=0;///////////////////
    int lenpath=0;/////////////////////////////
};

#endif // MODEL_H
