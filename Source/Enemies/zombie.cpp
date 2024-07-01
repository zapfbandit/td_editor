#include "Enemies/zombie.h"


Zombie::Zombie()
{}
\

Zombie::~Zombie()
{}


bool Zombie::Create(const double   x,
                    const double   y,
                    const double   dx,
                    const double   dy,
                    const double   scale)
{
   Sprite::Create(x, y, dx, dy, scale,
                  "Enemy",
                  "Zombie - Small",
                  6, 12, 12);

   return true;
}
