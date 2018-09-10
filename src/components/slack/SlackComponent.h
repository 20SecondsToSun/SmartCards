#ifndef SLACKCOMPONENT_H
#define SLACKCOMPONENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../BaseComponent.h"
#include "src/network/http/HTTPClient.h"

class SlackComponent : public BaseComponent
{
    Q_OBJECT
public:
    explicit SlackComponent(QObject *parent = nullptr);
    virtual ~SlackComponent();

    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;

    void sendMessage(const QString& msg, bool isError);

private:
   QSharedPointer<HTTPClient> httpClient;
   bool enabled = true;
   QString errorChannel = "";
   QString logChannel = "";
   int equestTimemoutInterval = 3000;

private slots:
   void httpRequestSuccessHandler(const QString& data);
   void httpRequestFailedHandler(const QString& data);

signals:
   void slackNotifyResponse(const QString& message);
};

#endif // SLACKCOMPONENT_H
