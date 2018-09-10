#include "TCPSocketClient.h"
#include <QTextCodec>
#include <QJsonArray>
#include <QStringList>

TCPSocketClient::TCPSocketClient():
    connectionStatus(ConnectionStatus::DISCONNECTED),
    waitForConnectionTime(3000),
    waitForReconnectionTime(3000)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),          this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),       this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),          this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    waitForConnectionTimer = new QTimer(this);
    waitForConnectionTimer->setSingleShot(true);
    waitForConnectionTimer->setInterval(waitForConnectionTime);
    connect(waitForConnectionTimer, SIGNAL(timeout()), this, SLOT(connectionTimerHandler()));

    waitForReconnectionTimer = new QTimer(this);
    waitForReconnectionTimer->setSingleShot(true);
    waitForReconnectionTimer->setInterval(waitForReconnectionTime);
    connect(waitForReconnectionTimer, SIGNAL(timeout()), this, SLOT(reconnectionTimerHandler()));
}

TCPSocketClient::~TCPSocketClient()
{
    if(socket)
    {
        socket->abort();
        killAllTimers();
        disconnect(socket, SIGNAL(connected()),          this, SLOT(connected()));
        disconnect(socket, SIGNAL(disconnected()),       this, SLOT(disconnected()));
        disconnect(socket, SIGNAL(readyRead()),          this, SLOT(readyRead()));
        disconnect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
        delete socket;
    }

    if(waitForConnectionTimer)
    {
        disconnect(waitForConnectionTimer, SIGNAL(timeout()), this, SLOT(connectionTimerHandler()));
        delete waitForConnectionTimer;
    }

    if(waitForReconnectionTimer)
    {
        disconnect(waitForReconnectionTimer, SIGNAL(timeout()), this, SLOT(reconnectionTimerHandler()));
        delete waitForReconnectionTimer;
    }
}


void TCPSocketClient::start()
{
    if(_autoConnect)
    {
        qDebug()<<"connect-------------------------------------------";
        connectToServer(_ip, _port);
    }
}

void TCPSocketClient::connected()
{   
    qDebug()<<"TCPSocketSender : connected";
    setConnectionStatus(ConnectionStatus::CONNECTED);
}

void TCPSocketClient::setConnectionStatus(ConnectionStatus status)
{
    connectionStatus = status;
    killAllTimers();

    switch(status)
    {
    case ConnectionStatus::CONNECTED:
        emit socketConnected();
        break;

    case ConnectionStatus::DISCONNECTED:
        emit socketDisonnected();
        qDebug()<<"ConnectionStatus::DISCONNECTED:  "<<_autoConnect;
        if(_autoConnect)
        {
            waitForReconnectionTimer->start();
        }
        break;

    case ConnectionStatus::CONNECTING:
        waitForConnectionTimer->start();
        break;

    }
    qDebug()<<"setConnectionStatus : "<<(int)connectionStatus;
    emit connectionStatusChanged(connectionStatus);
}

void TCPSocketClient::disconnected()
{   
    qDebug()<<"TCPSocketSender : disconnected";
    setConnectionStatus(ConnectionStatus::DISCONNECTED);
}

void TCPSocketClient::readyRead()
{
    emit socketDataRecieve(socket->readAll());
}

void TCPSocketClient::bytesWritten(qint64)
{
    qDebug()<<"TCPSocketSender : bytesWritten";
}

void TCPSocketClient::connectToServer(const QString& ip, int port)
{
    if(connectionStatus == ConnectionStatus::DISCONNECTED)
    {
        setIp(ip);
        setPort(port);
        qDebug()<<" start connection " <<_ip<<"  "<< _port;
        setConnectionStatus( ConnectionStatus::CONNECTING);
        socket->connectToHost(_ip, _port);
    }
}

void TCPSocketClient::disconnectSocket()
{
    if(connectionStatus != ConnectionStatus::DISCONNECTED && isOpen())
    {
        socket->abort();
    }
}

void TCPSocketClient::sendData(const QString& data)
{
    if(isOpen())
    {
        qDebug()<<"TCPSocketSender : write : " <<data.toStdString().c_str() ;
        try
        {
            QByteArray ba;
            ba.clear();
            ba.append(data);
            socket->write(ba);
        }
        catch(...)
        {
            qDebug()<<"error : ";
        }
    }
}

bool TCPSocketClient::isOpen() const
{
    return socket && socket->isOpen();
}

void TCPSocketClient::connectionTimerHandler()
{
    qDebug()<<"connection state! "<< socket->state();
    if(socket->state() == QAbstractSocket::SocketState::ConnectedState)
    {
        qDebug()<<"connection is fine!";
    }
    else
    {
        qDebug()<<"connection bad!";
        setConnectionStatus(ConnectionStatus::DISCONNECTED);
    }
}

void TCPSocketClient::reconnectionTimerHandler()
{
    connectToServer(_ip, _port);
}

void TCPSocketClient::killAllTimers()
{
    waitForConnectionTimer->stop();
    waitForReconnectionTimer->stop();
}

void TCPSocketClient::setDelimeter(const QString& value)
{
    delimeter = value;
}

void TCPSocketClient::setRunning(bool value)
{
    _running = value;
    emit runningChanged();
}

bool TCPSocketClient::running() const
{
    return _running;
}

void TCPSocketClient::setIp(const QString& value)
{
    _ip = value;
    emit ipChanged();
}

QString TCPSocketClient::ip() const
{
    return _ip;
}

void TCPSocketClient::setPort(int value)
{
    _port = value;
    emit portChanged();
}

int TCPSocketClient::port() const
{
    return _port;
}

void TCPSocketClient::setAutoConnect(bool value)
{
    _autoConnect = value;
    emit autoConnectChanged();
}

bool TCPSocketClient::autoConnect() const
{
    return _autoConnect;
}

void TCPSocketClient::setConnectionMeta(const QString& value)
{
    _connectionMeta = value;
    emit connectionMetaChanged();
}

QString TCPSocketClient::connectionMeta() const
{
    return _connectionMeta;
}

