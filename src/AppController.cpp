#include "AppController.h"
#include "modules/rfid/RFIDModule.h"
#include "modules/BaseModule.h"

AppController::AppController(QObject *parent) : QObject(parent)
{

}



void AppController::setQmlContext(QQmlContext* qmlContext)
{
    rfidModule->setQmlContext(qmlContext);
}

void AppController::init()
{
    rfidModule.reset(new RFIDModule());
    modules.push_back(rfidModule);

    for(auto module : modules)
    {
        module->init();
    }
}

void AppController::start()
{
    for(auto module : modules)
    {
        module->start();
    }
}

void AppController::stop()
{
    for(auto module : modules)
    {
        module->stop();
    }
}
