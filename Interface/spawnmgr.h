#pragma once

#include <stdint.h>
#include <vector>
#include <QTextStream>
#include <QString>
#include <QTreeWidget>


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

    struct EventInfo
    {
       uint32_t index_;
       uint32_t stage_;
       uint32_t percent_;
       uint32_t spawnIndex_;
       uint32_t numEnemies_;
       QString  enemyType_;
    };

public:

    uint32_t NumSpawns();
    SpawnMgr::SpawnInfo& GetSpawn(const uint32_t index);

    uint32_t NumEvents();
    SpawnMgr::EventInfo& GetEvent(const uint32_t index);

public:

   void SaveSpawns(QTextStream& stream, const MapView& map);
   void LoadSpawns(QTextStream& stream);

   void SaveEvents(QTextStream& stream, QTreeWidget* tree);
   void LoadEvents(QTextStream& stream);

public:

   void MakeSpawns();

private:

   SpawnDelegate& spawnDelegate_;
   SpriteMgr& spriteMgr_;
   uint32_t numStages_;
   std::vector<SpawnInfo> spawns_;
   std::vector<EventInfo> events_;
};
