#ifndef WINSCARDRFIDCOMPONENT_H
#define WINSCARDRFIDCOMPONENT_H

#include <QObject>
#include <QVector>
#include "src/components/rfid/BaseRFIDComponent.h"
#include "src/components/rfid/APDUCommand.h"

class WinscardRFIDComponent : public BaseRFIDComponent
{
    Q_OBJECT
public:
    explicit WinscardRFIDComponent(QObject *parent = nullptr);

    enum class WinscardError
    {
        CantStartTransaction,
        CantEndTransaction,
        LoadKeyError,
        NoCardReader,
        NoCard,
        AuthError,
        WriteError,
        ReadError,
        EmptyData,
        CardParsing,
        DataFormatError,
        UnknownError,
    };
    Q_ENUMS(WinscardError)

    virtual void start() override;
    virtual void stop() override;
    virtual void read() override;
    virtual void write(const QByteArray& data) override;

private:
    LoadkeyAPDUCommand loadkeyAPDUCommand;
    BlockAuthAPDUCommand blockAuthAPDUCommand;
    BeepAPDUCommand beepAPDUCommand;
    WriteAPDUCommand writeAPDUCommand;
    ReadAPDUCommand readAPDUCommand;

private:
    bool cardPreparedSuccess();
    bool establishContext();
    bool cardConnect();
    bool checkIsDeviceConnected();
    bool loadKey();
    bool blockAuthenticate(uint8_t blockNumber);
    bool beepCommand(bool enabled);
    bool writeBlockData(uint8_t blockNumber, const QByteArray& data);
    bool readBlockData(uint8_t blockNumber, QByteArray& data);
    void releaseCardReader();

private:
    void fillBlockAddresses();
    int getBlocksNeedForWriting(const QByteArray& cardData);

private:   
    SCARDCONTEXT cardContext;
    SCARDHANDLE cardHandle;

private:
    QVector<uint8_t> blockAddresses;

signals:
    void winscardError(WinscardRFIDComponent::WinscardError error);

public slots:
    virtual void onStartRead() override;
    virtual void onStartWrite(const QByteArray& data) override;
};

typedef WinscardRFIDComponent::WinscardError WinscardError;

#endif // WINSCARDRFIDCOMPONENT_H
