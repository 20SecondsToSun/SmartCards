#include "BaseComponent.h"

BaseComponent::BaseComponent(QObject *parent) : QObject(parent)
{

}

BaseComponent::~BaseComponent()
{

}

void BaseComponent::setQmlContext(QQmlContext* value)
{
    qmlContext = value;
    qmlContext->setContextProperty(name, this);
}

QString BaseComponent::getName() const
{
    return name;
}
