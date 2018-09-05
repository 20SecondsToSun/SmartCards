#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QQmlContext>
#include <QSharedPointer>

class RFIDModule;

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);

    void init();
    void start();
    void stop();

    void setQmlContext(QQmlContext* qmlContext);

    QSharedPointer<RFIDModule> rfidModule;

};

#endif // APPCONTROLLER_H
