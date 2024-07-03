#pragma once

#include <stdint.h>

#include <map>

#include <QString>

#include <QGraphicsScene>

#include "pixmapstore.h"

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
             PixmapStore*    store);

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

private:

   const uint32_t maxSprites_;
   Sprite* sprites_;

   std::map<QString, SpriteData> spriteIndexMap_;
};
