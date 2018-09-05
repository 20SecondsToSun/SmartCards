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
}

QString BaseComponent::getName() const
{
    return name;
}
