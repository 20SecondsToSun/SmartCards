#include "AppController.h"
#include "modules/RFIDModule.h"

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
    rfidModule->init();
}

void AppController::start()
{
    rfidModule->start();
}

void AppController::stop()
{
    rfidModule->stop();
}
