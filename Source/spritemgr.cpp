#include "spritemgr.h"

#include <QDebug>
#include <QString>

#include <QGraphicsScene>

#include "pixmapstore.h"


SpriteMgr::SpriteMgr(const uint32_t maxSprites):
   maxSprites_(maxSprites)
{
qDebug() << QString("About to allocate %0 sprites").arg(maxSprites);

   sprites_ = new Sprite[maxSprites_];

qDebug() << "OK";
}


SpriteMgr::~SpriteMgr()
{
qDebug() << "SpriteMgr::It's a killin' time...";

   freeSprites_.clear();
   usedSprites_.clear();
   delete [] sprites_;
}


void SpriteMgr::Init(QGraphicsScene* scene,
                     PixmapStore*    store,
                     MapView*        map)
{
qDebug() << "SpriteMgr::Init(...)";

   for (uint32_t i = 0; i < maxSprites_; ++i)
   {
      sprites_[i].Init(this, scene, store, map);
      freeSprites_.push_back(&sprites_[i]);
   }
}


uint32_t SpriteMgr::Add(const double   x,
                        const double   y,
                        const double   dx,
                        const double   dy,
                        const double   scale,

                        const QString& spriteType,
                        const QString& spriteName,
                        const uint32_t numFrames,
                        const double   framesPerSec,
                        const double   gridPerSec)
{
qDebug() << "SpriteMgr::Add(...)";

   if (freeSprites_.size() > 0)
   {
      Sprite* newSprite = freeSprites_.back();
      freeSprites_.pop_back();

      newSprite->Create(x, y, dx, dy, scale,
                         spriteType, spriteName, numFrames, framesPerSec, gridPerSec);

      usedSprites_.push_back(newSprite);

      return true;
   }

   return false;
}


void SpriteMgr::Tick(const double renderTimeInS)
{
//qDebug() << QString("SpriteMgr::Tick(renderTimeInS = %0)").arg(renderTimeInS);

   for (Sprite* sprite: usedSprites_)
   {
      sprite->Tick(renderTimeInS);
   }

   for (Sprite* sprite: deadSprites_)
   {
      //sprite->Destroy();
      FreeSprite(sprite);
   }
}


bool SpriteMgr::AddIndex(const QString& spritePath,
                         const uint32_t spriteIndex,
                         const uint32_t spriteNumFrames)
{
qDebug() << QString("SpriteMgr::AddIndex(\"%0\", %1, %2)").
               arg(spritePath).
               arg(spriteIndex).
               arg(spriteNumFrames);

   SpriteData spriteData = {spriteIndex, spriteNumFrames};

   return spriteIndexMap_.insert(std::make_pair(spritePath, spriteData)).second;
}


SpriteMgr::SpriteData* SpriteMgr::GetIndex(const QString& spritePath)
{
qDebug() << QString("SpriteMgr::GetIndex(spritePath = %0)").arg(spritePath);

   auto it = spriteIndexMap_.find(spritePath);
   if (it != spriteIndexMap_.end())
   {
      return &(it->second);
   }
   else
   {
      return nullptr;
   }
}


uint32_t SpriteMgr::NumSprites() const
{
   return usedSprites_.size();
}


void SpriteMgr::MarkForDeath(Sprite* sprite)
{
   deadSprites_.push_back(sprite);
}


void SpriteMgr::FreeSprite(Sprite* sprite)
{
   usedSprites_.remove(sprite);
   freeSprites_.push_front(sprite);
}
