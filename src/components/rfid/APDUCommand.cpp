#include "APDUCommand.h"

APDUCommand::~APDUCommand()
{
    if(commandBytes)
    {
        delete[] commandBytes;
    }
}

void APDUCommand::setCardHandle(SCARDHANDLE value)
{
    cardHandle = value;
}

void APDUCommand::setProtocol(LPCSCARD_IO_REQUEST value)
{
    protocol = value;
}

bool APDUCommand::perform()
{
    DWORD cbRecv = MAX_APDU_SIZE;
    BYTE pbRecv[MAX_APDU_SIZE];
    return SCardTransmit(cardHandle,
                         protocol,
                         commandBytes,
                         size,
                         NULL, pbRecv, &cbRecv) == SCARD_S_SUCCESS;
}

LoadkeyAPDUCommand::LoadkeyAPDUCommand()
{
    size = 11;
    commandBytes =  new uint8_t[size];
    commandBytes[0] = 0xff;
    commandBytes[1] = 0x82;
    commandBytes[2] = 0x00;
    commandBytes[3] = keyLocation; // key location
    commandBytes[4] = 0x06; // key length
    commandBytes[5] = 0xff;
    commandBytes[6] = 0xff;
    commandBytes[7] = 0xff;
    commandBytes[8] = 0xff;
    commandBytes[9] = 0xff;
    commandBytes[10] = 0xff;
}

void LoadkeyAPDUCommand::setKeyLocation(uint8_t value)
{
    commandBytes[3] = value;
}

void LoadkeyAPDUCommand::setKeyLength(uint8_t value)
{
    commandBytes[4] = value;
}

BlockAuthAPDUCommand::BlockAuthAPDUCommand()
{
    size = 10;
    commandBytes =  new uint8_t[size];
    commandBytes[0] = 0xff;
    commandBytes[1] = 0x86;
    commandBytes[2] = 0x00;
    commandBytes[3] = 0x00;
    commandBytes[4] = 0x05;
    commandBytes[5] = 0x01;
    commandBytes[6] = 0x00;
    commandBytes[7] = 0x00; //block to auth
    commandBytes[8] = keyType;  //key type
    commandBytes[9] = keyLocation; //key location
}

void BlockAuthAPDUCommand::setBlockNumber(uint8_t value)
{
    commandBytes[7] = value;
}

void BlockAuthAPDUCommand::setKeyType(uint8_t value)
{
    commandBytes[8] = value;
}

void BlockAuthAPDUCommand::setKeyLocation(uint8_t value)
{
    commandBytes[9] = value;
}

BeepAPDUCommand::BeepAPDUCommand()
{
    size = 5;
    commandBytes =  new uint8_t[size];
    commandBytes[0] = 0xff;
    commandBytes[1] = 0x00;
    commandBytes[2] = 0x52;
    commandBytes[3] = 0x00;//beep byte
    commandBytes[4] = 0x00;
}

void BeepAPDUCommand::setEnabled(bool value)
{
    commandBytes[3] = value ? 0xff: 0x00;
}

WriteAPDUCommand::WriteAPDUCommand()
{

    size = reserveBytes + 16;
    commandBytes =  new uint8_t[size];

    commandBytes[0] = 0xff;
    commandBytes[1] = 0xD6;
    commandBytes[2] = 0x00;
    commandBytes[3] = 0x00; //block number
    commandBytes[4] = 0x10;
}

void WriteAPDUCommand::setBlockNumber(uint8_t value)
{
    commandBytes[3] = value;
}

void WriteAPDUCommand::setData(const QByteArray& data)
{
    for(int i = reserveBytes; i < size; i++)
    {
        commandBytes[i] = data[i - reserveBytes];
    }
}

ReadAPDUCommand::ReadAPDUCommand()
{
    uint8_t bytesNum = 0x010;
    size = 5;
    commandBytes =  new uint8_t[size];

    commandBytes[0] = 0xff;
    commandBytes[1] = 0xB0;
    commandBytes[2] = 0x00;
    commandBytes[3] = 0x00; //blockNumber
    commandBytes[4] = bytesNum; //bytesNum
}

void ReadAPDUCommand::setBlockNumber(uint8_t value)
{
    commandBytes[3] = value;
}

QByteArray ReadAPDUCommand::getData()
{
    return lastReadedData;
}

bool ReadAPDUCommand::perform()
{
    DWORD cbRecv = MAX_APDU_SIZE;
    BYTE pbRecv[MAX_APDU_SIZE];

    bool status = SCardTransmit(cardHandle,
                                protocol,
                                commandBytes,
                                size,
                                NULL, pbRecv, &cbRecv) == SCARD_S_SUCCESS;

    if(status && cbRecv >= 2)
    {
        lastReadedData.clear();

        for (int i = 0; i < cbRecv - 2; i++)
        {
            lastReadedData.append(pbRecv[i]);
        }
    }

    return status;
}
