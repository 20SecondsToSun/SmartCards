#include "RFIDModule.h"
#include "src/components/rfid/WinscardRFIDComponent.h"
#include "src/components/rfid/BaseRFIDComponent.h"

RFIDModule::RFIDModule(QObject *parent) : QObject(parent)
{

}

void RFIDModule::setQmlContext(QQmlContext* qmlContext)
{
    qmlContext->setContextProperty("smartCard", this);
}

template <class RFIDComponentT>
void RFIDModule::inject()
{
    rfid.reset(new RFIDComponentT());
}

void RFIDModule::init()
{
    inject<WinscardRFIDComponent>();
    rfid->setDeviceName(deviceName);

    message = new RfidMessage();

    rfidThread = new QThread();
    connect(rfidThread, SIGNAL(started()), this, SLOT(onRfidThreadStarted()));
    connect(rfidThread, SIGNAL(finished()), rfidThread, SLOT(deleteLater()));

    connect(rfid.data(), SIGNAL(dataReaded(const QByteArray&)), this, SLOT(onDataReaded(const QByteArray&)));
    connect(rfid.data(), SIGNAL(dataWrited()), this, SLOT(onDataWrited()));
    connect(rfid.data(), SIGNAL(winscardError(WinscardError )), this, SLOT(onWinscardError(WinscardError )));

    rfid->moveToThread(rfidThread);
    rfidThread->start();

    connect(this, SIGNAL(startRead()), rfid.data(), SLOT(onStartRead()));
    connect(this, SIGNAL(startWrite(const QByteArray&)), rfid.data(), SLOT(onStartWrite(const QByteArray&)));
}

void RFIDModule::start()
{

}

void RFIDModule::stop()
{

}

void RFIDModule::read()
{
    if(!isBusy())
    {
        setIsBusy(true);
        emit startRead();
        //rfid->read();
    }
}

bool RFIDModule::isBusy() const
{
    return _isBusy;
}

void RFIDModule::setIsBusy(bool value)
{
    _isBusy = value;
    emit isBusyChanged();
}

void RFIDModule::write(const QString& data)
{
    if(!isBusy())
    {
        setIsBusy(true);
        emit startWrite(message->prepareBytesForWrite(data, messageType));
    }
}

void RFIDModule::onRfidThreadStarted()
{
    qDebug()<<"onRfidThreadStarted ........";
}

void RFIDModule::onDataReaded(const QByteArray& data)
{
    setIsBusy(false);
    emit dataReaded(message->parseBytes(data, messageType));
}

void RFIDModule::onDataWrited()
{
    setIsBusy(false);
    qDebug()<<"on write success ........";
}

void RFIDModule::onWinscardError(WinscardError error)
{
    setIsBusy(false);
    qDebug()<<"error ........"<<(int)error;
}

