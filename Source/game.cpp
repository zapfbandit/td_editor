#include "game.h"


Game::Game(SpriteMgr& spriteMgr):
   spriteMgr_(spriteMgr)
{
   timer_.start();
}


void Game::Tick()
{
   const double time = timer_.elapsed() / 1000.0;

   static double s_lastTime = -1;
   if (s_lastTime == -1)

   s_lastTime = time;

//qDebug() << time - s_lastTime;

   spriteMgr_.Tick(time - s_lastTime);

   s_lastTime = time;
}
