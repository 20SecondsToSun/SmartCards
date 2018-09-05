#ifndef RFIDMODULE_H
#define RFIDMODULE_H

#include <QObject>
#include <QQmlContext>
#include <QString>
#include <QSharedPointer>
#include "src/components/rfid/WinscardRFIDComponent.h"

class BaseRFIDComponent;

class RFIDModule : public QObject
{
    Q_OBJECT

public:
    RFIDModule();

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

private slots:
    void onDataReaded(const QString& data);
    void onDataWrited();
    void onWinscardError(WinscardError error);
};

#endif // RFIDMODULE_H
