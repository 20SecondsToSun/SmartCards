#include "BaseModule.h"

BaseModule::BaseModule(QObject *parent) : QObject(parent)
{

}

BaseModule::~BaseModule()
{

}

void BaseModule::setQmlContext(QQmlContext* value)
{
    qmlContext = value;
    qmlContext->setContextProperty(name, this);
}

QString BaseModule::getName() const
{
    return name;
}
