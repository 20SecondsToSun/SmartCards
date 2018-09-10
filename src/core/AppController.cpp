#include "AppController.h"
#include "../modules/BaseModule.h"
#include "../components/slack/SlackComponent.h"
#include "../components/logging/LoggerComponent.h"
#include "../components/monitoring/MonitoringComponent.h"
#include "../modules/rfid/RFIDModule.h"
#include "../modules/BaseModule.h"
#include "../screens/GameScreen.h"

AppController::AppController(QObject *parent) : QObject(parent)
{

}

void AppController::setQmlContext(QQmlContext* qmlContext)
{
    rfidModule->setQmlContext(qmlContext);
}

void AppController::init()
{
    slackComponent.reset(new SlackComponent());
    components.push_back(slackComponent);

    loggerComponent.reset(new LoggerComponent());
    components.push_back(loggerComponent);

    monitoringComponent.reset(new MonitoringComponent());
    components.push_back(monitoringComponent);

    gameScreen.reset(new GameScreen());
    screens.push_back(gameScreen);

    rfidModule.reset(new RFIDModule());
    modules.push_back(rfidModule);

    for(auto comp : components)
    {
        comp->init();
    }

    for(auto module : modules)
    {
        module->init();
    }

    for(auto screen : screens)
    {
        screen->init();
    }
}

void AppController::start()
{
    for(auto comp : components)
    {
        comp->start();
    }

    for(auto module : modules)
    {
        module->start();
    }

    for(auto screen : screens)
    {
        screen->start();
    }


}

void AppController::stop()
{
    for(auto comp : components)
    {
        comp->stop();
    }

    for(auto module : modules)
    {
        module->stop();
    }

    for(auto screen : screens)
    {
        screen->stop();
    }
}

QUrl AppController::getStartQML() const
{
    return QUrl(QLatin1String("qrc:/qml/main.qml"));
}
