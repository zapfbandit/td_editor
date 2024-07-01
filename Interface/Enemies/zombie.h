#pragma once

#include "sprite.h"

class Zombie : public Sprite
{
public:

   Zombie();
   ~Zombie();

private:

   virtual bool Create(const double   x,
                       const double   y,
                       const double   dx,
                       const double   dy,
                       const double   scale) final;
};
