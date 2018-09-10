#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QObject>
#include "BaseScreen.h"

class GameScreen : public BaseScreen
{
    Q_OBJECT
public:
    explicit GameScreen(QObject *parent = nullptr);
    virtual ~GameScreen();

    virtual void init() override;
    virtual void start() override;
    virtual void stop() override;

};

#endif // GAMESCREEN_H
