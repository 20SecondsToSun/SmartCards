#ifndef RFIDMODULE_H
#define RFIDMODULE_H

#include <QObject>
#include <QQmlContext>
#include <QString>
#include <QThread>
#include <QSharedPointer>
#include "src/components/rfid/WinscardRFIDComponent.h"

class BaseRFIDComponent;

class RFIDModule : public QObject
{
    Q_OBJECT

public:
    explicit RFIDModule(QObject *parent = nullptr);

    template <class RFIDComponentT>
    void inject();

    void init();
    void start();
    void stop();
    void setQmlContext(QQmlContext* qmlContext);

    Q_INVOKABLE void read();
    Q_INVOKABLE void write(const QString& data);

private:
    QSharedPointer<BaseRFIDComponent> rfid;
    QThread* rfidThread = nullptr;

private slots:
    void onDataReaded(const QString& data);
    void onDataWrited();
    void onWinscardError(WinscardError error);

    void onRfidThreadStarted();

signals:
    void dataReaded(const QString& data);
};

#endif // RFIDMODULE_H
