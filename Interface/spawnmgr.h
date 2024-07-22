#pragma once

#include <stdint.h>
#include <vector>
#include <QTextStream>


class SpawnDelegate;
class SpriteMgr;
class MapView;


class SpawnMgr
{
public:

   SpawnMgr(SpawnDelegate& spawnDelegate,
            SpriteMgr& spriteMgr);
   
public:

   struct SpawnInfo
   {
      uint32_t index_;
      double x_;
      double y_;
      double dx_;
      double dy_;
   };


public:

   uint32_t NumSpawns();
   SpawnMgr::SpawnInfo& GetSpawn(const uint32_t num);

public:

   void SaveSpawns(QTextStream& stream, const MapView& map);
   void LoadSpawns(QTextStream& stream);

public:

   void MakeSpawns();

private:

   SpawnDelegate& spawnDelegate_;
   SpriteMgr& spriteMgr_;

   std::vector<SpawnInfo> spawns_;
};
