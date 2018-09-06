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
    virtual void write(const QByteArray& data) = 0;

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
    void setCardType(CardType cardType);
    void setEndSymbol(QChar value);
    void setDeviceName(const QString& name);

protected:
    RFIDMode _rfidMode = RFIDMode::Reading;
    CardType _cardType = CardType::MifareClassic1K;
    CardProtocol _cardProtocol = CardProtocol::Raw;
    QChar endSymbol;

    bool _connected = false;
    QString deviceName;

signals: 
    void connectedChanged();
    void rfidModeChanged(RFIDMode mode);
    void cardTypeChanged(CardType cardType);

    void dataReaded(const QString& data);
    void dataReaded(const QByteArray& data);
    void dataWrited();
    void errorOccured();
};

typedef BaseRFIDComponent::RFIDMode RFIDMode;

#endif // RFIDCOMPONENT_H
