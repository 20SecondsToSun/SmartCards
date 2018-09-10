#ifndef MONITORINGCOMPONENT_H
#define MONITORINGCOMPONENT_H

#include <QObject>
#include <QTimer>

#include "../BaseComponent.h"
#include "../monitoring/MemoryChecker.h"
#include "../logging/LoggerComponent.h"

class MonitoringComponent : public BaseComponent
{
    Q_OBJECT

    Q_PROPERTY(int memory READ memory WRITE setMemory NOTIFY memoryChanged)

public:
    explicit MonitoringComponent(QObject *parent = nullptr);
    virtual ~MonitoringComponent();

    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;

    int memory() const;
    void setMemory(int value);

private:
   QTimer* timer;
   MemoryChecker memoryChecker;
   QSharedPointer<LoggerComponent> loggerComponent;
   int _memory = 0;
   bool enabled = true;
   int memoryCheckMills = 30000;

signals:
   void memoryChanged();

private slots:
    void onUpdate();
};

#endif // MONITORINGCOMPONENT_H
