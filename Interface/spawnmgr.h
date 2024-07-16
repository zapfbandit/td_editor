#pragma once

#include <stdint.h>


class SpawnMgr
{
public:

   SpawnMgr();
   
public:

   struct SpawnInfo
   {
      uint32_t index_;
      double x_;
      double y_;
      double dx_;
      double dy_;
   };
};
