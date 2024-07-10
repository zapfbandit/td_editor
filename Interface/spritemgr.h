#pragma once

#include <stdint.h>

#include <map>
#include <list>

#include <QString>

#include <QGraphicsScene>

#include "pixmapstore.h"
#include "mapview.h"

#include "sprite.h"


class SpriteMgr
{
public:

   SpriteMgr(const uint32_t  maxSprites);

   ~SpriteMgr();

public:

   struct SpriteData
   {
      const uint32_t firstAnimFrameIndex_;
      const uint32_t numAnimFrames_;
   };

public:

   void Init(QGraphicsScene* scene,
             PixmapStore*    store,
             MapView*        map);

   uint32_t Add(const double   x,
                const double   y,
                const double   dx,
                const double   dy,
                const double   scale,

                const QString& spriteType,
                const QString& spriteName,
                const uint32_t numFrames,
                const double   framesPerSec,
                const double   gridPerSec);

   void Tick(const double renderTimeInS);

   bool AddIndex(const QString& spritePath,
                 const uint32_t spriteIndex,
                 const uint32_t spriteNumFrames);

   SpriteData* GetIndex(const QString& spritePath);

   uint32_t NumSprites() const;

   void FreeSprite(Sprite* sprite);

private:

   const uint32_t maxSprites_;
   Sprite* sprites_;

   std::map<QString, SpriteData> spriteIndexMap_;

   std::list<Sprite*> freeSprites_;
   std::list<Sprite*> usedSprites_;
};
