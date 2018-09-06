#include "WinscardRFIDComponent.h"
#include <QGuiApplication>

WinscardRFIDComponent::WinscardRFIDComponent(QObject *parent) : BaseRFIDComponent(parent)
{
    fillBlockAddresses();
}

void WinscardRFIDComponent::start()
{

}

void WinscardRFIDComponent::stop()
{

}

void WinscardRFIDComponent::onStartRead()
{
    read();
}

void WinscardRFIDComponent::read()
{
    if(!cardPreparedSuccess())
    {
        return;
    }

    if(SCardBeginTransaction(cardHandle) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantStartTransaction);
        return;
    }

    QByteArray fulldata;

    for(int i = 0; i < blockAddresses.size(); i++)
    {
        uint8_t blockAddress = blockAddresses[i];

        if(!blockAuthenticate(blockAddress))
        {
            releaseCardReader();
            emit winscardError(WinscardError::AuthError);
            return;
        }

        QByteArray data;

        if(!readBlockData(blockAddress, data))
        {
            releaseCardReader();
            emit winscardError(WinscardError::ReadError);
            return;
        }

        //        if(data.isEmpty())
        //        {
        //            releaseCardReader();
        //            emit winscardError(WinscardError::EmptyData);
        //            return;
        //        }

        fulldata.append(data);
    }

    qDebug()<<"fulldata "<<QString(fulldata);
    qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    if(SCardEndTransaction(cardHandle, SCARD_LEAVE_CARD) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantEndTransaction);
        return;
    }

    releaseCardReader();
    emit dataReaded(fulldata);
}


void WinscardRFIDComponent::onStartWrite(const QByteArray& data)
{
    write(data);
}

void WinscardRFIDComponent::write(const QByteArray& data)
{
    if(!cardPreparedSuccess())
    {
        return;
    }

    if(SCardBeginTransaction(cardHandle) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantStartTransaction);
        return;
    }

    qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    QByteArray cardData = data;
    int blocksNeeded = getBlocksNeedForWriting(cardData);
    int blockOffset = 0;
    const int MAX_BLOCK_FOR_WRITE = blockAddresses.size();

    if(blocksNeeded > MAX_BLOCK_FOR_WRITE)
    {
        releaseCardReader();
        emit winscardError(WinscardError::WriteError);
        return;
    }

    for(int i = 0; i < blocksNeeded; i++)
    {
        uint8_t blockAdress = blockAddresses[i];

        if(!blockAuthenticate(blockAdress))
        {
            releaseCardReader();
            emit winscardError(WinscardError::AuthError);
            return;
        }

        QByteArray blockData;
        const int ONE_BLOCK_SIZE = 16;

        for(int i = 0; i < ONE_BLOCK_SIZE; i++)
        {
            blockData[i] = cardData[blockOffset + i];
        }
        blockOffset += ONE_BLOCK_SIZE;

        qDebug()<<"write to block: "<<blockAdress<<" : "<<blockData;

        if(!writeBlockData(blockAdress, blockData))
        {
            releaseCardReader();
            emit winscardError(WinscardError::WriteError);
            return;
        }
    }

    if(SCardEndTransaction(cardHandle, SCARD_LEAVE_CARD) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantEndTransaction);
        return;
    }

    emit dataWrited();
}

bool WinscardRFIDComponent::cardPreparedSuccess()
{
    bool cardPrepared = true;
    bool cardReaderStatus = true;

    if(!establishContext())
    {
        cardReaderStatus = false;
        cardPrepared = false;
        emit winscardError(WinscardError::NoCardReader);
    }
    else if(!checkIsDeviceConnected())
    {
        cardReaderStatus = false;
        cardPrepared = false;
        emit winscardError(WinscardError::NoCardReader);
    }
    else if(!cardConnect())
    {
        cardPrepared = false;
        emit winscardError(WinscardError::NoCard);
    }
    else if(!loadKey())
    {
        cardPrepared = false;
        emit winscardError(WinscardError::LoadKeyError);
    }

    if(!cardPrepared)
    {
        releaseCardReader();
    }

    return cardPrepared;
}

bool WinscardRFIDComponent::establishContext()
{
    return SCardEstablishContext(SCARD_SCOPE_USER, 0, 0, &cardContext) == SCARD_S_SUCCESS;
}

bool WinscardRFIDComponent::cardConnect()
{
    DWORD dwAP = 0;
    LPCWSTR name = reinterpret_cast<LPCWSTR>(deviceName.unicode());
    return SCardConnect(cardContext, name, SCARD_SHARE_SHARED, SCARD_PROTOCOL_Tx, &cardHandle, &dwAP) == SCARD_S_SUCCESS;
}

bool WinscardRFIDComponent::checkIsDeviceConnected()
{
    LPWSTR mszReaders;
    DWORD dwReaders = SCARD_AUTOALLOCATE;

    if(SCardListReadersW(cardContext, NULL, (LPWSTR)&mszReaders, &dwReaders) != SCARD_S_SUCCESS)
    {
        return false;
    }

    QByteArray buffer;
    wchar_t *it = mszReaders;

    while(*it !='\0')
    {
        buffer.append(QChar(*it));
        *it++;
    }
    QString name(buffer);

    if(SCardFreeMemory(cardContext, mszReaders)!= SCARD_S_SUCCESS)
    {
        return false;
    }

    return name.contains(deviceName);
}

bool WinscardRFIDComponent::loadKey()
{
    loadkeyAPDUCommand.setCardHandle(cardHandle);
    return loadkeyAPDUCommand.perform();
}

bool WinscardRFIDComponent::blockAuthenticate(uint8_t blockNumber)
{
    blockAuthAPDUCommand.setCardHandle(cardHandle);
    blockAuthAPDUCommand.setBlockNumber(blockNumber);
    return blockAuthAPDUCommand.perform();
}

bool WinscardRFIDComponent::beepCommand(bool enabled)
{
    beepAPDUCommand.setCardHandle(cardHandle);
    beepAPDUCommand.setEnabled(enabled);
    return beepAPDUCommand.perform();
}

bool WinscardRFIDComponent::writeBlockData(uint8_t blockNumber, const QByteArray& data)
{
    writeAPDUCommand.setCardHandle(cardHandle);
    writeAPDUCommand.setBlockNumber(blockNumber);
    writeAPDUCommand.setData(data);
    return writeAPDUCommand.perform();
}

bool WinscardRFIDComponent::readBlockData(uint8_t blockNumber, QByteArray& data)
{
    readAPDUCommand.setCardHandle(cardHandle);
    readAPDUCommand.setBlockNumber(blockNumber);
    bool status = readAPDUCommand.perform();

    if(status)
    {
        data = readAPDUCommand.getData();
    }

    return status;
}

void WinscardRFIDComponent::releaseCardReader()
{
    SCardDisconnect(cardHandle, SCARD_UNPOWER_CARD);
    SCardReleaseContext(cardContext);
}

void WinscardRFIDComponent::fillBlockAddresses()
{
    //fill adress array with 64 blocks start adresses
    //every third block - trail block, does'n use
    //read milfare specs

    //mifare classick 1K

    int k = 0;
    const int allBlocksNum = 64;
    const int hexBase = 16;

    blockAddresses.clear();

    for(int i = 0; i < allBlocksNum; i++)
    {
        if(k++ == 3)
        {
            k = 0;
        }
        else
        {
            QString hex = QString::number(i, hexBase);
            bool ok;
            const unsigned int parsedValue = hex.toUInt(&ok, hexBase);
            blockAddresses.push_back(parsedValue);
        }
    }
    blockAddresses.pop_front();//don't use 0x00 block
}

int WinscardRFIDComponent::getBlocksNeedForWriting(const QByteArray& cardData)
{
    const int ONE_BLOCK_SIZE = 16;
    int addBlock = cardData.size() % ONE_BLOCK_SIZE != 0 ? 1 : 0;
    return cardData.size() / ONE_BLOCK_SIZE + addBlock;
}
