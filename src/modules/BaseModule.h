#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <QObject>
#include <QQmlContext>

class BaseModule : public QObject
{
    Q_OBJECT
public:
    explicit BaseModule(QObject *parent = nullptr);
    virtual ~BaseModule();

    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void setQmlContext(QQmlContext* qmlContext);
    virtual QString getName() const;

protected:
    QQmlContext* qmlContext;
    QString name;
};

#endif // BASEMODULE_H
