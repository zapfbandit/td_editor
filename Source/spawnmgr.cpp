#include "spawnmgr.h"

#include "spawndelegate.h"
#include "spritemgr.h"
#include "mapview.h"


SpawnMgr::SpawnMgr(SpawnDelegate& spawnDelegate,
                   SpriteMgr&     spriteMgr):
   spawnDelegate_(spawnDelegate),
   spriteMgr_(spriteMgr)
{}


void SpawnMgr::MakeSpawns()
{
   uint32_t numSpawns = NumSpawns();

   if (numSpawns > 0)
   {
      for (uint32_t i = 0; i < numSpawns; ++i)
      {
         SpawnMgr::SpawnInfo spawn = GetSpawn(i);

         double x = spawn.x_ + spawn.dx_ + 0.5;
         double dx = -spawn.dx_;
         double y = spawn.y_ + spawn.dy_ + 0.5;
         double dy = -spawn.dy_;

qDebug() << i << x << y << dx << dy;

         for (uint32_t i = 0; i < 1; ++i)
         {
            double px = 0;
            double py = 0;
            do
            {
               px = (rand() - RAND_MAX/2.0) / (RAND_MAX/2.0);
               py = (rand() - RAND_MAX/2.0) / (RAND_MAX/2.0);
            }
            while (px*px + py*py > 1);

            Sprite* newSprite = new Sprite(x + 0.2*px, y + 0.2*py, dx, dy, 0.5, "Enemies", "Zombie", 6, 10, 0.2);
            spriteMgr_.Add(newSprite);
         }
      }
   }
   else
   {
      //spriteMgr_.Add(0.5, 0.5, 1, 0, 1, "Enemies", "Zombie", 6, 2, 0.1);

      //spriteMgr_.Add(0.5, 0.5, 0, 1, 2, "Enemies", "Zombie - Big", 6, 2, 0.1);
   }
}


void SpawnMgr::SaveSpawns(QTextStream& stream, const MapView& map)
{
   spawns_.clear();
   uint32_t index = 0;
   uint32_t tileId = 0;

   for (uint32_t y = 0; y < map.Height(); ++y)
   {
      tileId = map.GetTile(0, y) - 1;
      if ((tileId != -1) && ((tileId & 0x8) == 0))
      {
         spawns_.push_back({index++, 0, (double)y, -1, 0});
      }

      tileId = map.GetTile(map.Width() - 1, y) - 1;
      if ((tileId != -1) && ((tileId & 0x2) == 0))
      {
         spawns_.push_back({index++, (double)(map.Width() - 1), (double)y, 1, 0});
      }
   }

   for (uint32_t x = 0; x < map.Width(); ++x)
   {
      tileId = map.GetTile(x, 0) - 1;
      if ((tileId != -1) && ((tileId & 0x1) == 0))
      {
         spawns_.push_back({index++, (double)x, 0, 0, -1});
      }

      tileId = map.GetTile(x, map.Height() - 1) - 1;
      if ((tileId != -1) && ((tileId & 0x4) == 0))
      {
         spawns_.push_back({index++, (double)x, (double)(map.Height() - 1), 0, 1});
      }
   }

   stream << spawns_.size() << "\r\n";

   uint32_t i = 0;
   for (auto it: spawns_)
   {
      stream << it.index_ << it.x_ << it.y_ << it.dx_ << it.dy_ << "\r\n";
   }
}


void SpawnMgr::SaveEvents(QTextStream& stream, QTreeWidget* tree)
{
   qDebug() << "SpawnMgr::SaveEvents";

   int numStages = tree->topLevelItemCount();

   stream << numStages << "\r\n";
   stream << "Events\r\n";

   int numEvents = 0;

   for (int stage = 0; stage < numStages; ++stage)
   {
      QTreeWidgetItem* stageItem = tree->topLevelItem(stage);
      numEvents += stageItem->childCount();
   }

   stream << numEvents << "\r\n";

   uint32_t eventId = 0;

   for (int stage = 0; stage < numStages; ++stage)
   {
      QTreeWidgetItem* stageItem = tree->topLevelItem(stage);
      int numChildren = stageItem->childCount();

      for (int event = 0; event < numChildren; ++event)
      {
         QTreeWidgetItem* eventItem = stageItem->child(event);

         stream << eventId++
                << (stage + 1)
                << eventItem->data(1, Qt::DisplayRole).toUInt()
                << spawnDelegate_.SpawnIndex(eventItem->data(2, Qt::DisplayRole).toString())
                << eventItem->data(3, Qt::DisplayRole).toUInt()
                << ""
                << eventItem->data(4, Qt::DisplayRole).toString()
                << "\r\n";
      }
   }
}


void SpawnMgr::LoadSpawns(QTextStream& stream)
{
   spawns_.clear();

   uint32_t numSpawns = 0;
   stream >> numSpawns;

   for (uint32_t i = 0; i < numSpawns; ++i)
   {
      uint32_t index;
      double x, y;
      double dx, dy;
      stream >> index >> x >> y >> dx >> dy;
      spawns_.push_back({index, x, y, dx, dy});
   }

   spawnDelegate_.SetSpawns(spawns_);

//*
for (auto it: spawns_)
{
qDebug() << it.index_ << "(" << it.x_ << it.y_ << ") (" << it.dx_ << it.dy_ <<  ")";
}
//*/
}

void SpawnMgr::LoadEvents(QTextStream& stream, QSpinBox* numStagesSpinBox, QTreeWidget* tree)
{
   events_.clear();

   uint32_t numStages = 0;
   stream >> numStages;

   QString eventsStr; // Should be "Events"
   stream >> eventsStr;

   uint32_t numEvents = 0;
   stream >> numEvents;

   if (numStagesSpinBox != nullptr)
   {
      numStagesSpinBox->setValue(numStages);
   }

   tree->clear();

   QTreeWidgetItem** topLevelItems;
   topLevelItems = new QTreeWidgetItem*[numStages];

   for (uint32_t i = 0; i < numStages; ++i)
   {
      topLevelItems[i] = new QTreeWidgetItem(QStringList() << QString::number(i + 1));
      tree->addTopLevelItem(topLevelItems[i]);
   }

   for (uint32_t i = 0; i < numEvents; ++i)
   {
      uint32_t index = 0;
      uint32_t stage = 0;
      uint32_t percent = 0;
      uint32_t spawnIndex = 0;
      uint32_t numEnemies = 0;
      QString  enemyType = "";

      stream >> index >> stage >> percent >> spawnIndex >> numEnemies >> enemyType;
      events_.push_back({index, stage, percent, spawnIndex, numEnemies, enemyType});

      QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "" << QString::number(percent) << spawnDelegate_.SpawnStr(spawnIndex) << QString::number(numEnemies) << enemyType);
      item->setFlags(item->flags() | Qt::ItemIsEditable);

      topLevelItems[stage - 1]->addChild(item);
   }

   tree->expandAll();

   delete [] topLevelItems;
}


uint32_t SpawnMgr::NumSpawns()
{
   return spawns_.size();
}


SpawnMgr::SpawnInfo& SpawnMgr::GetSpawn(const uint32_t num)
{
   return spawns_[num];
}
