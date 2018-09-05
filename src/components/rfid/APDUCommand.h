#ifndef APDUCOMMAND_H
#define APDUCOMMAND_H

#include <QByteArray>
#include <QDebug>
#include "stdint.h"
#include "WINSCARD.H"

struct APDUCommand
{
protected:
    static const DWORD MAX_APDU_SIZE = 255;
    DWORD size = 0;
    LPCSCARD_IO_REQUEST protocol = SCARD_PCI_T1;
    uint8_t keyType = 0x60; //0x60 /*TypeA */ 0x61 /*TypeB */
    uint8_t keyLocation = 0x00;//0x01
    SCARDHANDLE card_handle_;
    uint8_t* commandBytes;

public:
    virtual ~APDUCommand();
    virtual bool perform();

    void setCardHandle(SCARDHANDLE value);
    void setProtocol(LPCSCARD_IO_REQUEST value);
};

struct LoadkeyAPDUCommand : APDUCommand
{
    LoadkeyAPDUCommand();

    void setKeyLocation(uint8_t value);
    void setKeyLength(uint8_t value);
};

struct BlockAuthAPDUCommand : APDUCommand
{
    BlockAuthAPDUCommand();

    void setBlockNumber(uint8_t value);
    void setKeyLocation(uint8_t value);
    void setKeyType(uint8_t value);
};

struct BeepAPDUCommand : APDUCommand
{
    BeepAPDUCommand();

    void setEnabled(bool value);
};

struct WriteAPDUCommand : APDUCommand
{
private:
    const int reserveBytes = 5;

public:
    WriteAPDUCommand();

    void setBlockNumber(uint8_t value);
    void setData(const QByteArray& data);
};

struct ReadAPDUCommand : APDUCommand
{
private:
    QByteArray lastReadedData;

public:
    ReadAPDUCommand();
    void setBlockNumber(uint8_t value);
    QByteArray getData();
    virtual bool perform() override;
};

#endif // APDUCOMMAND_H
