#pragma once

#include <QString>

#include <QGraphicsPixmapItem>

#include <QGraphicsScene>

#include "pixmapStore.h"
#include "mapview.h"

class SpriteMgr;

class Sprite
{
public:

   Sprite();

   ~Sprite();

public:

   void Init(SpriteMgr*      mgr,
             QGraphicsScene* scene,
             PixmapStore*    store,
             MapView*        map);

   virtual bool Create(const double   x,
                       const double   y,
                       const double   dx,
                       const double   dy,
                       const double   scale) { return false; }

   bool Create(const double   x,
               const double   y,
               const double   dx,
               const double   dy,
               const double   scale,

               const QString& spriteType,
               const QString& spriteName,
               const uint32_t numFrames,
               const double   framesPerSec,
               const double   gridPerSec);

public:

   void SetPos(const double x,
               const double y);

   void SetVel(const double dx,
               const double dy);

public:

   virtual void Tick(const double renderTimeInSec);

private:

   SpriteMgr* mgr_;
   QGraphicsScene* scene_;
   PixmapStore* store_;
   MapView* map_;

   double timeInSec_;

   QString spriteType_;
   QString spriteName_;
   QString spriteMove_;
   QString spriteDir_;

   uint32_t frame_;

   double x_;
   double y_;
   double dx_;
   double dy_;
   double scale_;

   uint32_t baseFrameIndex_;

   uint32_t numFrames_;

   double framesPerSec_;
   double secsPerFrame_;
   double gridPerSec_;

   double dist_;

   QGraphicsPixmapItem* item_;
};

