#ifndef LOGGERSERVICE_H
#define LOGGERSERVICE_H

#include <QObject>
#include <QQmlContext>
#include "../BaseComponent.h"
#include "../slack/SlackComponent.h"

enum class LogType
{
    Verbose,
    Warning,
    Error
};

enum class LogRemoteType
{
    None,
    Server,
    Slack,
};

class LoggerComponent : public BaseComponent
{
    Q_OBJECT

public:
    LoggerComponent(QObject *parent = nullptr);
    virtual ~LoggerComponent();

    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;

    void log(const QString& message, LogType type, LogRemoteType remoteType, bool saveLocal = true);

    QString createSlackMessage(const QString& message) const;
    QString createLocalMessage(const QString& message) const;
    QString getLocalLogAbsoluteFilePath() const;
    QString getLocalLogDirPath() const;

private:
    QSharedPointer<SlackComponent> slackComponent;
    QString logFormat = "dd.MM.yyyy";
    bool localEnabled = true;
    QString localPath = "logs";

    void logTofile(const QString& message);

private slots:
    void onSlackNotifyResponse(const QString& message);
};

#endif // LOGGERSERVICE_H
