#include "WinscardRFIDComponent.h"

WinscardRFIDComponent::WinscardRFIDComponent(QObject *parent) : BaseRFIDComponent(parent)
{
    cardReaderName = "ACS ACR122 0";
    fillBlockAdresses();
}

void WinscardRFIDComponent::start()
{

}

void WinscardRFIDComponent::stop()
{

}

void WinscardRFIDComponent::read()
{
    if(!cardPreparedSuccess())
    {
        return;
    }

    if(SCardBeginTransaction(card_handle_) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantStartTransaction);
        return;
    }

    QByteArray fulldata;

    for(int i = 0; i < blockAdresses.size(); i++)
    {
        uint8_t blockAdress = blockAdresses[i];

        if(!blockAuthenticate(blockAdress))
        {
            releaseCardReader();
            emit winscardError(WinscardError::AuthError);
            return;
        }

        QByteArray data;

        if(!readBlockData(blockAdress, data))
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

    if(SCardEndTransaction(card_handle_, SCARD_LEAVE_CARD) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantEndTransaction);
        return;
    }

    releaseCardReader();
    emit dataReaded(QString(fulldata));
}

void WinscardRFIDComponent::write(const QString& data)
{
    if(!cardPreparedSuccess())
    {
        return;
    }

    if(SCardBeginTransaction(card_handle_) != SCARD_S_SUCCESS)
    {
        releaseCardReader();
        emit winscardError(WinscardError::CantStartTransaction);
        return;
    }

    qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    QByteArray cardData = data.toUtf8();
    int blocksNeeded = getBlocksNeedForWriting(cardData);
    int blockOffset = 0;
    const int MAX_BLOCK_FOR_WRITE = blockAdresses.size();

    if(blocksNeeded > MAX_BLOCK_FOR_WRITE)
    {
        releaseCardReader();
        emit winscardError(WinscardError::WriteError);
        return;
    }

    for(int i = 0; i < blocksNeeded; i++)
    {
        uint8_t blockAdress = blockAdresses[i];

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

    if(SCardEndTransaction(card_handle_, SCARD_LEAVE_CARD) != SCARD_S_SUCCESS)
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
    return SCardEstablishContext(SCARD_SCOPE_USER, 0, 0, &card_context_) == SCARD_S_SUCCESS;
}

bool WinscardRFIDComponent::cardConnect()
{
    DWORD dwAP = 0;
    LPCWSTR _cardReaderName = reinterpret_cast<LPCWSTR>(cardReaderName.unicode());
    return SCardConnect(card_context_, _cardReaderName, SCARD_SHARE_SHARED, SCARD_PROTOCOL_Tx, &card_handle_, &dwAP) == SCARD_S_SUCCESS;
}

bool WinscardRFIDComponent::checkIsDeviceConnected()
{
    LPWSTR mszReaders;
    DWORD dwReaders = SCARD_AUTOALLOCATE;

    if(SCardListReadersW(card_context_, NULL, (LPWSTR)&mszReaders, &dwReaders) != SCARD_S_SUCCESS)
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

    if(SCardFreeMemory(card_context_, mszReaders)!= SCARD_S_SUCCESS)
    {
        return false;
    }

    return name.contains(cardReaderName);
}

bool WinscardRFIDComponent::loadKey()
{
    loadkeyAPDUCommand.setCardHandle(card_handle_);
    return loadkeyAPDUCommand.perform();
}

bool WinscardRFIDComponent::blockAuthenticate(uint8_t blockNumber)
{
    blockAuthAPDUCommand.setCardHandle(card_handle_);
    blockAuthAPDUCommand.setBlockNumber(blockNumber);
    return blockAuthAPDUCommand.perform();
}

bool WinscardRFIDComponent::beepCommand(bool enabled)
{
    beepAPDUCommand.setCardHandle(card_handle_);
    beepAPDUCommand.setEnabled(enabled);
    return beepAPDUCommand.perform();
}

bool WinscardRFIDComponent::writeBlockData(uint8_t blockNumber, const QByteArray& data)
{
    writeAPDUCommand.setCardHandle(card_handle_);
    writeAPDUCommand.setBlockNumber(blockNumber);
    writeAPDUCommand.setData(data);
    return writeAPDUCommand.perform();
}

bool WinscardRFIDComponent::readBlockData(uint8_t blockNumber, QByteArray& data)
{
    readAPDUCommand.setCardHandle(card_handle_);
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
    SCardDisconnect(card_handle_, SCARD_UNPOWER_CARD);
    SCardReleaseContext(card_context_);
}

void WinscardRFIDComponent::fillBlockAdresses()
{
    //fill adress array with 64 blocks start adresses
    //every third block - trail block, does'n use
    //read milfare specs

    int k = 0;
    const int allBlocksNum = 64;
    const int hexBase = 16;

    blockAdresses.clear();

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
            blockAdresses.push_back(parsedValue);
        }
    }
    blockAdresses.pop_front();//don't use 0x00 block
}

int WinscardRFIDComponent::getBlocksNeedForWriting(const QByteArray& cardData)
{
    const int ONE_BLOCK_SIZE = 16;
    int addBlock = cardData.size() % ONE_BLOCK_SIZE != 0 ? 1 : 0;
    return cardData.size() / ONE_BLOCK_SIZE + addBlock;
}
