#include "BaseRFIDComponent.h"

BaseRFIDComponent::BaseRFIDComponent(QObject *parent) : BaseComponent(parent)
{
    name = "SmartCard";
}

BaseRFIDComponent::~BaseRFIDComponent()
{

}

void BaseRFIDComponent::start()
{

}

void BaseRFIDComponent::stop()
{

}

void BaseRFIDComponent::setConnected(bool value)
{
    _connected = value;
    emit connectedChanged();
}

bool BaseRFIDComponent::connected() const
{
    return _connected;
}

void BaseRFIDComponent::setRFIDMode(RFIDMode value)
{
    _rfidMode = value;
    emit rfidModeChanged(value);
}

void BaseRFIDComponent::setCardType(CardType value)
{
    _cardType = value;
    emit cardTypeChanged(value);
}

void BaseRFIDComponent::setEndSymbol(QChar value)
{
    endSymbol = value;
}




