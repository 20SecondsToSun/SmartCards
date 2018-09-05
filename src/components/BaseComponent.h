#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <QObject>
#include <QQmlContext>

class BaseComponent : public QObject
{
    Q_OBJECT
public:
    explicit BaseComponent(QObject *parent = nullptr);
    virtual ~BaseComponent();
    virtual void setQmlContext(QQmlContext* value);

    virtual QString getName() const;
    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    QQmlContext* qmlContext;
    QString name = "";
};

#endif // BASECOMPONENT_H
