#ifndef RFIDMODULE_H
#define RFIDMODULE_H

#include <QObject>
#include <QQmlContext>
#include <QString>
#include <QThread>
#include <QSharedPointer>
#include <QNdefMessage>
#include "src/components/rfid/WinscardRFIDComponent.h"
#include "src/components/rfid/RfidMessage.h"

#include "src/modules/BaseModule.h"

class BaseRFIDComponent;

class RFIDModule : public BaseModule
{
    Q_OBJECT

public:
    explicit RFIDModule(QObject *parent = nullptr);
    virtual ~RFIDModule();

    Q_PROPERTY(bool isBusy READ isBusy WRITE setIsBusy NOTIFY isBusyChanged)

    template <class RFIDComponentT>
    void inject();

    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;

    Q_INVOKABLE void read();
    Q_INVOKABLE void write(const QString& data);

    bool isBusy() const;
    void setIsBusy(bool value);

private:
    RfidMessageType messageType = RfidMessageType::RawData;
    QString deviceName = "ACS ACR122 0";

private:
    QSharedPointer<BaseRFIDComponent> rfid;
    QThread* rfidThread = nullptr;
    RfidMessage* message;
    bool _isBusy = false;

    void ndefMessageRead(const QNdefMessage &message);

private slots:
    void onDataReaded(const QByteArray& data);
    void onDataWrited();
    void onWinscardError(WinscardRFIDComponent::WinscardError error);

    void onRfidThreadStarted();

signals:
    void dataReaded(const QString& data);
    void startRead();
    void startWrite(const QByteArray&);
    void isBusyChanged();

};

#endif // RFIDMODULE_H
