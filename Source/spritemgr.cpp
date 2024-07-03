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

   delete [] sprites_;
}


void SpriteMgr::Init(QGraphicsScene* scene,
                     PixmapStore*    store)
{
qDebug() << "SpriteMgr::Init(...)";

   for (uint32_t i = 0; i < maxSprites_; ++i)
   {
      sprites_[i].Init(scene, store);
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

   for (uint32_t i = 0; i < maxSprites_; ++i)
   {
      if (sprites_[i].Used() == false)
      {
         sprites_[i].Create(x, y, dx, dy, scale,
                            spriteType, spriteName, numFrames, framesPerSec, gridPerSec);

         return true;
      }
   }

   return false;
}


void SpriteMgr::Tick(const double renderTimeInS)
{
//qDebug() << QString("SpriteMgr::Tick(renderTimeInS = %0)").arg(renderTimeInS);

   for (uint32_t i = 0; i < maxSprites_; ++i)
   {
      if (sprites_[i].Used() == true)
      {
         sprites_[i].Tick(renderTimeInS);
      }
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
