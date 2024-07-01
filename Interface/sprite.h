#pragma once

#include <QString>

#include <QGraphicsPixmapItem>

#include <QGraphicsScene>

#include "pixmapStore.h"


class Sprite
{
public:

   Sprite();

   ~Sprite();

public:

   void Init(QGraphicsScene* scene,
             PixmapStore*    store);

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
               const double   pixelsPerSec);

public:

   void SetPos(const double x,
               const double y);

   void SetVel(const double dx,
               const double dy);

public:

   bool Used() const;

public:

   virtual void Tick(const double renderTimeInSec);

private:

   void UpdateFrame(const uint32_t frame);

   void UpdatePos(const double x,
                  const double y);

private:

   bool used_;

   QGraphicsScene* scene_;
   PixmapStore* store_;

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
   double pixelsPerSec_;

   QGraphicsPixmapItem* item_;
};

