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


///-------Create autopilot and move to thread with timer----------------------------------------
    autopilot = new Autopilot();
    autopilot->init(100);

    threadAutopilot = new QThread();

    timerAutopilot = new QTimer(0);
    timerAutopilot->setInterval(autopilot->getMSecDeltaTime());
    timerAutopilot->moveToThread(threadAutopilot);

    // вызываем слот loop() по таймеру
    autopilot->connect( timerAutopilot, SIGNAL(timeout()), SLOT(loop()), Qt::ConnectionType::DirectConnection);

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
//    model.connect(gps, SIGNAL(updatePositionXY(const double&, const double&)),
    //                         SLOT(acceptCoordXY(const double&, const double&)), Qt::QueuedConnection );

    // изменение координат передаются в viewData для дальнейшего отображения
    // например как пары чисел
//        viewData.connect(gps, SIGNAL(updatePositionXY(const double&, const double&)),
//                         SLOT(acceptCoord(const double&, const double&)), Qt::QueuedConnection );
///----------------------------------------------------------------------------------------------



///-------Connects objects-----------------------------------------------------------------------

    // связываем обновление положения в автопилоте с чтением положения в gps
    autopilot->connect(gps      , SIGNAL(updatePositionXY(const double&, const double&)),
                       SLOT(readFromGPS(const double&, const double&)) );

    // получаем ключевую точку в автопилот из model (полученную из QML)
    // для добавления в список ключевых точек
    autopilot->connect(&model, SIGNAL(sendKeyPointForAdding(const QVector2D&)),
                       SLOT(addKeyPoint(const QVector2D&)) );

    // изменение пути и ключевых точек в автопилоте передаются в viewData
    // для дальнейшего отображения
    model.connect(autopilot, SIGNAL(signalAppPointToPathAndRemoveFirst(const QVector2D&)),
                                SLOT(slotAppPointToPathAndRemoveFirst(const QVector2D&)) );
    model.connect(autopilot, SIGNAL(signalAppPointToPath(const QVector2D&)),
                                SLOT(slotAppPointToPath(const QVector2D&)) );

    model.connect(autopilot, SIGNAL(keyPointsChanged(const ListVector&)),
                                SLOT(acceptKeyPoints(const ListVector&)) );

///----------------------------------------------------------------------------------------------

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("modelView", &model);
    context->setContextProperty("fieldModel", fieldModel);
    context->setContextProperty("fieldDataBase", &dbField);
    context->setContextProperty("appSettings", &settings);

    qmlRegisterType<TypeEdit>("TypeEdit", 1, 0, "TypeEdit");
    qmlRegisterType<DriveMode>("DriveMode", 1, 0, "DriveMode");
    qmlRegisterType<DrawTrack>("DrawTrack",1,0,"DrawTrack");//регистрируем
    qRegisterMetaType<ListVector>("ListVector");// в метаобъектной системе

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
    //threadControllerI2C_14->start();
///----------------------------------------------------------------------------------------------

    return app.exec();
}
