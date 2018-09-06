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

    rfid->moveToThread(rfidThread);
    rfidThread->start();

    connect(rfid.data(), SIGNAL(dataReaded(const QByteArray&)), this, SLOT(onDataReaded(const QByteArray&)));
    connect(rfid.data(), SIGNAL(dataWrited()), this, SLOT(onDataWrited()));
    connect(rfid.data(), SIGNAL(winscardError(WinscardError )), this, SLOT(onWinscardError(WinscardError )));
}

void RFIDModule::start()
{

}

void RFIDModule::stop()
{

}

void RFIDModule::read()
{
    rfid->read();
}

void RFIDModule::write(const QString& data)
{    
    rfid->write(message->prepareBytesForWrite(data, messageType));
}

void RFIDModule::onRfidThreadStarted()
{
    qDebug()<<"onRfidThreadStarted ........";
}

void RFIDModule::onDataReaded(const QByteArray& data)
{
   emit dataReaded(message->parseBytes(data, messageType));
}

void RFIDModule::onDataWrited()
{
    qDebug()<<"on write success ........";
}

void RFIDModule::onWinscardError(WinscardError error)
{
    qDebug()<<"error ........"<<(int)error;
}

