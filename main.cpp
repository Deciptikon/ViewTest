#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "model.h"
#include "fieldobject.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    Model model;

    QList<QObject*> listField;
    listField.append(new FieldObject("field model 1", "12345"));
    listField.append(new FieldObject("field model 2", "22222"));
    listField.append(new FieldObject("field model 33", "54321"));
    listField.append(new FieldObject("field model 45", "45546"));

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("TestModel", &model);
    context->setContextProperty("fieldModel", QVariant::fromValue(listField));



    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
