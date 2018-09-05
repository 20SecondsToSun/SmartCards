#ifndef RFIDCOMPONENT_H
#define RFIDCOMPONENT_H

#include <QObject>
#include "src/components/BaseComponent.h"

class BaseRFIDComponent : public BaseComponent
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)

public:
    explicit BaseRFIDComponent(QObject *parent = nullptr);
    virtual ~BaseRFIDComponent();

    virtual void start() override;
    virtual void stop() override;

    virtual void read() = 0;
    virtual void write(const QString& data) = 0;

    enum class RFIDMode
    {
        Reading,
        Writing,
        Stopped,
        Validating
    };
    Q_ENUMS(RFIDMode)

    enum class CardType
    {
        MifareClassic1K,
        MifareClassic4K
    };
    Q_ENUMS(CardType)

    enum class CardProtocol
    {
        Raw,
        NDEF
    };
    Q_ENUMS(CardProtocol)

    void setConnected(bool value);
    bool connected() const;
    void setRFIDMode(RFIDMode rfidMode);

protected:
    RFIDMode _rfidMode = RFIDMode::Reading;
    CardType _cardType = CardType::MifareClassic1K;
    CardProtocol _cardProtocol = CardProtocol::Raw;

    bool _connected = false;

signals: 
    void connectedChanged();
    void rfidModeChanged(RFIDMode mode);

    void dataReaded(const QString& data);
    void dataWrited();
    void errorOccured();
};

typedef BaseRFIDComponent::RFIDMode RFIDMode;

#endif // RFIDCOMPONENT_H
