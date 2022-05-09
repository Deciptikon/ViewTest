#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "appsettings.h"
#include "model.h"
#include "databasefield.h"
#include "fieldmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    // настройки приложения
    AppSettings settings;
    // Теперь можно получить их по ключу
    //qreal s = settings.getValueQreal(TEST_NAME);

    QGuiApplication app(argc, argv);

    Model model;

    DataBaseField dbField;// база данных
    dbField.connectToDataBase();

    FieldModel *fieldModel = new FieldModel();// модель для отображения базы данных

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("TestModel", &model);
    context->setContextProperty("fieldModel", fieldModel);
    context->setContextProperty("fieldDataBase", &dbField);
    context->setContextProperty("appSettings", &settings);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
