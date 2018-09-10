#include "HTTPClient.h"
#include <QThread.h>
#include <QTextCodec.h>
#include "../../tools/StringTools.h"

HTTPClient::HTTPClient(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    networkManager->moveToThread(QThread::currentThread());

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpRequestSuccessHandler(QNetworkReply*)));
    //connect(networkManager, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpRequestErrorHandler(QNetworkReply::NetworkError)));
    connect(networkManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)), this, SLOT(sslErrorHandler(QNetworkReply*, QList<QSslError>)));

    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeoutHandle()));
}

HTTPClient::~HTTPClient()
{
    if(networkManager)
    {
        disconnect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpRequestSuccessHandler(QNetworkReply*)));
        //disconnect(networkManager, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpRequestErrorHandler(QNetworkReply::NetworkError)));
        disconnect(networkManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)), this, SLOT(sslErrorHandler(QNetworkReply*, QList<QSslError>)));

        delete networkManager;
    }

    if(timeoutTimer)
    {
        disconnect(timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeoutHandle()));
        delete timeoutTimer;
    }
}

void HTTPClient::setTimemoutInterval(int value)
{
    requestTimemoutInterval = value;
    timeoutTimer->setInterval(value);
}

void HTTPClient::setRequestTryCount(int value)
{
    requestTryCount = value;
}

void HTTPClient::onTimeoutHandle()
{
    qDebug()<<"============== On Server Timeout Handle ============= ";
    if(httpReply)
    {        
        httpReply->close();
    }
}

void HTTPClient::runGetRequest(const QNetworkRequest& request)
{
    httpReply = networkManager->get(request);
    timeoutTimer->start(requestTimemoutInterval);
}

void HTTPClient::runPostRequest(const QNetworkRequest& request, const QByteArray& data)
{  
    httpReply = networkManager->post(request, data);  
    timeoutTimer->start(requestTimemoutInterval);
}

void HTTPClient::runPutRequest(const QNetworkRequest& request, const QByteArray& data)
{
    httpReply = networkManager->put(request, data);
    timeoutTimer->start(requestTimemoutInterval);
}

void HTTPClient::runDeleteRequest(const QNetworkRequest& request)
{    
    httpReply = networkManager->deleteResource(request);
    timeoutTimer->start(requestTimemoutInterval);
}

void HTTPClient::httpRequestSuccessHandler(QNetworkReply* reply)
{
    timeoutTimer->stop();
    if (reply->error() != QNetworkReply::NoError )
    {
        qDebug() << "Request failed, " << reply->readAll();
        emit httpRequestFailed(reply->errorString());
    }
    else
    {
        QByteArray ba = reply->readAll();
        StringTools stringTools;
        QString modifyedString = stringTools.convertUnicodeToCyrillic(ba);
        emit httpRequestSuccess(modifyedString);
    }

    reply->deleteLater();
}

void HTTPClient::httpRequestErrorHandler(QNetworkReply::NetworkError data)
{
    timeoutTimer->stop();
    emit httpRequestFailed("Request error");
}

void HTTPClient::sslErrorHandler(QNetworkReply* reply, const QList<QSslError> &errors)
{
    timeoutTimer->stop();

    foreach (const QSslError& e, errors)
    {
        qDebug() << "SSL Error:" << e.errorString();
    }

    reply->ignoreSslErrors(errors);
    emit httpRequestFailed("SSL Error");
}
