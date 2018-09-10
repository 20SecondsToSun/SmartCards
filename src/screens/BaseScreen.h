#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QObject>

class BaseScreen : public QObject
{
    Q_OBJECT
public:
    explicit BaseScreen(QObject *parent = nullptr);

    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif // BASESCREEN_H
