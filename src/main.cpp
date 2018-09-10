#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "core/AppController.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QScopedPointer<AppController> appController(new AppController);
    engine.rootContext()->setContextProperty("appController", appController.data());

    appController->init();
    appController.data()->setQmlContext(engine.rootContext());
    appController->start();

    engine.load(appController->getStartQML());

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
