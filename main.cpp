#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QThread>

#include "constants.h"
#include "TypeEdit.h"
#include "DriveMode.h"

#include "appsettings.h"
#include "model.h"
#include "databasefield.h"
#include "fieldmodel.h"
#include "autopilot.h"
#include "gps.h"
#include "drawtrack.h"
#include "devicei2c.h"
#include "sensorreader.h"

typedef QList<QVector2D> ListVector;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QSettings sett(ORGANIZATION_NAME, APPLICATION_NAME);
    QVariant re = sett.value(DIR_RESTORE_SETTINGS KEY_RESTORE_SETTINGS,
                                  DEFAULT_RESTORE_SETTINGS );
    if(re.toBool()) {
        AppSettings s;
        // проверяем и востаннавливаем настройки (при необходимости)
        s.checkAndRestoreSettings();
    }


    QTranslator translator;// переводчик
    QVariant tr = sett.value(DIR_LANGUAGE KEY_CURRENT_LANGUAGE,
                                  DEFAULT_CURRENT_LANGUAGE );
    translator.load(QString("QtLanguage_") + QString(tr.toString()), ".");// выбираем локализацию


    QGuiApplication app(argc, argv);
    app.installTranslator(&translator);// устанавливаем перевод в приложение


    // настройки приложения(загружаются после app что бы сработал перевед)
    AppSettings settings;


    Model model;


    DataBaseField dbField;// база данных
    dbField.connectToDataBase();


    FieldModel *fieldModel = new FieldModel();// модель для отображения базы данных

    Autopilot *autopilot;
    QThread *threadAutopilot;
    QTimer *timerAutopilot;

    GPS *gps;
    QThread *threadGPS;

    DeviceI2C *devicei2c_14;
    QThread *threadDeviceI2C_14;

    SensorReader *sensorreader;
    QThread *threadSensorReader;
    QTimer *timerSensorReader;


///-------Create autopilot and move to thread with timer----------------------------------------
    autopilot = new Autopilot();
    autopilot->init(100);

    threadAutopilot = new QThread();

    timerAutopilot = new QTimer(0);
    timerAutopilot->setInterval(autopilot->getMSecDeltaTime());
    timerAutopilot->moveToThread(threadAutopilot);

    // вызываем слот loop() по таймеру
    autopilot->connect( timerAutopilot, SIGNAL(timeout()), SLOT(loop()),
                        Qt::ConnectionType::DirectConnection);

    // запускаем таймер как только поток стартует
    timerAutopilot->connect(threadAutopilot, SIGNAL(started()), SLOT(start()));
///----------------------------------------------------------------------------------------------



///-------Create GPS-reader----------------------------------------------------------------------
    gps = new GPS();

    threadGPS = new QThread();

    gps->moveToThread(threadGPS);

    // инициализируем gps как serialport device как только поток стартует
    gps->connect(threadGPS, SIGNAL(started()), SLOT(init()) );

    model.connect(gps, SIGNAL(gpsOn()), SLOT(slotGPSon()) );
    model.connect(gps, SIGNAL(gpsOff()), SLOT(slotGPSoff()) );


///----------------------------------------------------------------------------------------------


///-------Create I2C slave device----------------------------------------------------------------
    devicei2c_14 = new DeviceI2C();
    devicei2c_14->init( 0x14 );

    threadDeviceI2C_14 = new QThread();
    devicei2c_14->moveToThread(threadDeviceI2C_14);
///----------------------------------------------------------------------------------------------



///-------Create Sensor Reader----------------------------------------------------------------
    sensorreader = new SensorReader();
    sensorreader->init( 100 );//потом использовать интервал из настроек

    threadSensorReader = new QThread();

    timerSensorReader = new QTimer(0);
    timerSensorReader->setInterval(sensorreader->getMsecDeltaTime());
    timerSensorReader->moveToThread(threadSensorReader);

    // вызываем слот loop() по таймеру
    sensorreader->connect( timerSensorReader, SIGNAL(timeout()),
                           SLOT(loop()), Qt::ConnectionType::DirectConnection);

    // запускаем таймер как только поток стартует
    timerSensorReader->connect(threadSensorReader, SIGNAL(started()), SLOT(start()));
///----------------------------------------------------------------------------------------------




///-------Connects objects-----------------------------------------------------------------------

    // связываем обновление положения в автопилоте с чтением положения в gps
    autopilot->connect(gps      , SIGNAL(updatePositionXY(double,double)),
                       SLOT(readFromGPS(double,double)) );

    // получаем ключевую точку в автопилот из model (полученную из QML)
    // для добавления в список ключевых точек
    autopilot->connect(&model, SIGNAL(sendKeyPointForAdding(QVector2D)),
                       SLOT(addKeyPoint(QVector2D)) );

    // получаем режим вождения
    autopilot->connect(&model, SIGNAL(sendDriveMode(QVariant)),
                       SLOT(acceptDriveMode(QVariant)) );

    // получаем сигнал на установку точек А и В
    autopilot->connect(&model, SIGNAL(signalSetPointA()), SLOT(slotSetPointA()) );
    autopilot->connect(&model, SIGNAL(signalSetPointB()), SLOT(slotSetPointB()) );

    // получаем данные с акселерометра и гироскопа
    autopilot->connect(sensorreader, SIGNAL(updateDataSens(QVector3D,QVector3D)),
                       SLOT(readFromGyroAndAccel(QVector3D,QVector3D)) );

    // изменение пути и ключевых точек в автопилоте передаются в model
    // для дальнейшего отображения
//    model.connect(autopilot, SIGNAL(signalAppPointToPathAndRemoveFirst(const QVector2D&)),
//                                SLOT(slotAppPointToPathAndRemoveFirst(const QVector2D&)) );
    model.connect(autopilot, SIGNAL(signalAppPointToPath(QVector2D)),
                  SLOT(slotAppPointToPath(QVector2D)) );

    model.connect(autopilot, SIGNAL(keyPointsChanged(ListVector)),
                  SLOT(acceptKeyPoints(ListVector)) );


    // устанавливаем точку А и направление (по точке В)
    model.connect(autopilot, SIGNAL(sendPointAToDraw(QVector2D)),
                  SLOT(addPointAToQML(QVector2D)) );
    model.connect(autopilot, SIGNAL(sendDirectToDraw(QVector2D)),
                  SLOT(addDirectToQML(QVector2D)) );

    // отправка команды на slave14
    devicei2c_14->connect(autopilot, SIGNAL(sendCommandToSlave14(int)),
                          SLOT(writeData(int)) );
    devicei2c_14->connect(&model, SIGNAL(signalCommandToSlave14(int)),
                          SLOT(writeData(int)) );

    // передаем данные сенсоров в QML
    model.connect(sensorreader, SIGNAL(updateDataSens(QVector3D,QVector3D)),
                  SLOT(slotDataSensToQML(QVector3D,QVector3D)),
                  Qt::ConnectionType::DirectConnection);

    // связываем сигналы и слоты на управление калибровкой
    // положения покоя акселерометра
    sensorreader->connect(&model, SIGNAL(signalCalibrateZeroPointAccelerometer(int)),
                          SLOT(slotCalibrateZeroPointAccelerometer(int)) );
    // передача успешного завершения калибровки в QML
    model.connect(sensorreader, SIGNAL(signalCalibrateZeroPointAccelerometerIsDone()),
                  SLOT(slotCalibrateZeroPointAccelerometerIsDone()) );

    // положения покоя гироскопа
    sensorreader->connect(&model, SIGNAL(signalCalibrateZeroPointGyroscope(int)),
                          SLOT(slotCalibrateZeroPointGyroscope(int)) );
    // передача успешного завершения калибровки в QML
    model.connect(sensorreader, SIGNAL(signalCalibrateZeroPointGyroscopeIsDone()),
                  SLOT(slotCalibrateZeroPointGyroscopeIsDone()) );

    // положение оси Z гироскопа
    sensorreader->connect(&model, SIGNAL(signalCalibrateZAxisGyroscope()),
                          SLOT(slotCalibrateZAxisGyroscope()) );
    // передача успешного завершения калибровки оси Z в QML
    model.connect(sensorreader, SIGNAL(signalCalibrateZAxisGyroscopeIsDone()),
                  SLOT(slotCalibrateZAxisGyroscopeIsDone()) );

    // положение оси X акселерометра
    sensorreader->connect(&model, SIGNAL(signalCalibrateXAxisAccelerometer()),
                          SLOT(slotCalibrateXAxisAccelerometer()) );
    // передача успешного завершения калибровки оси X в QML
    model.connect(sensorreader, SIGNAL(signalCalibrateXAxisAccelerometerIsDone()),
                  SLOT(slotCalibrateXAxisAccelerometerIsDone()) );

///----------------------------------------------------------------------------------------------

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("modelView", &model);
    context->setContextProperty("fieldModel", fieldModel);
    context->setContextProperty("fieldDataBase", &dbField);
    context->setContextProperty("appSettings", &settings);

    //регистрируем в метаобъектной системе
    qmlRegisterType<TypeEdit>("TypeEdit", 1, 0, "TypeEdit");
    qmlRegisterType<DriveMode>("DriveMode", 1, 0, "DriveMode");
    qmlRegisterType<DrawTrack>("DrawTrack",1,0,"DrawTrack");
    qRegisterMetaType<ListVector>("ListVector");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

///-------Start threads--------------------------------------------------------------------------
    threadAutopilot->start();
    threadGPS->start();
    threadDeviceI2C_14->start();
    threadSensorReader->start();
///----------------------------------------------------------------------------------------------

    return app.exec();
}
