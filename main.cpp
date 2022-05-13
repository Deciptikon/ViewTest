#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

#include "constants.h"
#include "TypeEdit.h"
#include "appsettings.h"
#include "model.h"
#include "databasefield.h"
#include "fieldmodel.h"

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

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("model", &model);
    context->setContextProperty("fieldModel", fieldModel);
    context->setContextProperty("fieldDataBase", &dbField);
    context->setContextProperty("appSettings", &settings);

    qmlRegisterType<TypeEdit>("TypeEdit", 1, 0, "TypeEdit");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
