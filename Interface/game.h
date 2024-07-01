#pragma once

#include "spritemgr.h"

#include <QElapsedTimer>


class Game
{
public:

   Game(SpriteMgr& spriteMgr);

public slots:

   void Tick();

private:

   SpriteMgr& spriteMgr_;
   QElapsedTimer timer_;
};
