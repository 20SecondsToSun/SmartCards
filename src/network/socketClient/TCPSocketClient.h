#ifndef TCPSOCKETSENDER_H
#define TCPSOCKETSENDER_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>

class TCPSocketClient : public QObject
{
    Q_OBJECT 

public:
    enum class ConnectionStatus
    {
        NONE,
        CONNECTING,
        CONNECTED,
        DISCONNECTED
    };
    Q_ENUMS(ConnectionStatus)

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64);
    virtual void readyRead();

signals:
    void socketConnected();
    void socketDisonnected();
    void connectionStatusChanged(TCPSocketClient::ConnectionStatus status);

signals:
    void runningChanged();
    void ipChanged();
    void portChanged();
    void autoConnectChanged();
    void connectionMetaChanged();
    void socketDataRecieve(const QByteArray& data);

public:
    TCPSocketClient();
    virtual ~TCPSocketClient();

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(QString connectionMeta READ connectionMeta WRITE setConnectionMeta NOTIFY connectionMetaChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool autoConnect READ autoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)

    bool running() const;
    QString ip() const;
    QString connectionMeta() const;
    int port() const;
    bool autoConnect() const;

    void setRunning(bool value);
    void setIp(const QString& value);
    void setConnectionMeta(const QString& value);
    void setPort(int value);
    void setAutoConnect(bool value);

    Q_INVOKABLE void connectToServer(const QString& ip, int port);
    Q_INVOKABLE void sendData(const QString& data);

    Q_INVOKABLE void disconnectSocket();
    Q_INVOKABLE bool isOpen() const;

    void setDelimeter(const QString& value);

    virtual void start();

protected:
    QTcpSocket* socket;

    int waitForConnectionTime;
    int waitForReconnectionTime;

    bool _running = false;
    QString _ip;
    QString _connectionMeta;
    int _port;
    bool _autoConnect;
    QString delimeter;

    void setConnectionStatus(ConnectionStatus status);

private:
    QTimer* waitForConnectionTimer;
    QTimer* waitForReconnectionTimer;
    ConnectionStatus connectionStatus;

    void killAllTimers();

private slots:
    void connectionTimerHandler();
    void reconnectionTimerHandler();
};

#endif // TCPSOCKETSENDER_H
