#include "sprite.h"

#include <QGraphicsScene>
#include "pixmapstore.h"


Sprite::Sprite():
   used_(false),
   item_(nullptr)
{}


Sprite::~Sprite()
{}



void Sprite::Init(QGraphicsScene* scene,
                  PixmapStore*    store,
                  MapView*        map)
{
   scene_ = scene;
   store_ = store;
   map_   = map;
}


bool Sprite::Create(const double   x,
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
   used_ = true;

   frame_ = 0;

   spriteType_ = spriteType;
   spriteName_ = spriteName;
   spriteMove_ = "Walk";

   SetVel(dx, dy);

   scale_ = scale;

   numFrames_    = numFrames;
   framesPerSec_ = framesPerSec;
   secsPerFrame_ = 1.0 / framesPerSec_;
   gridPerSec_   = gridPerSec;

   timeInSec_ = secsPerFrame_ * numFrames_ * rand() / RAND_MAX;

   QPixmap& pixmap = *store_->GetPixmap(baseFrameIndex_);

   // Centre the sprite

   SetPos(x - 0.5 * scale,
          y - 0.5 * scale);

   lastX_ = (int32_t)x;
   lastY_ = (int32_t)y;

//qDebug() << "Pos" << x << y;

   item_ = scene_->addPixmap(pixmap);
   item_->setScale(scale / pixmap.width());
   item_->setPos(x, y);


   Tick(0.0);

   return true;
}


void Sprite::SetPos(const double x,
                    const double y)
{
   x_ = x;
   y_ = y;
}


void Sprite::SetVel(const double dx,
                    const double dy)
{
   dx_ = dx;
   dy_ = dy;

   if (abs(dx) >= abs(dy))
   {
      if (dx >= 0)
      {
         spriteDir_ = "R";
      }
      else
      {
         spriteDir_ = "L";
      }
   }
   else
   {
      if (dy >= 0)
      {
         spriteDir_ = "D";
      }
      else
      {
         spriteDir_ = "U";
      }
   }

   baseFrameIndex_ = store_->GetPixmapIndex(QString("%0/%1/Walk/%2").
                                             arg(spriteType_).
                                             arg(spriteName_).
                                             arg(spriteDir_));
}


bool Sprite::Used() const
{
   return used_;
}


void Sprite::Tick(const double renderTimeInSec)
{
//qDebug() << QString("Sprite::Tick(renderTimeInSec = %0)").arg(renderTimeInSec);

   uint32_t lastFrame = frame_;

   timeInSec_ += renderTimeInSec;

   frame_ = static_cast<uint32_t>(timeInSec_ / secsPerFrame_);

   frame_ %= numFrames_;

   frame_ += baseFrameIndex_;

   if (frame_ != lastFrame)
   {
      item_->setPixmap(*store_->GetPixmap(frame_));
   }

//qDebug() << "scale_ = " << scale_ << "item_->scale() = " << item_->scale();

   if (dx_ != 0.0)
   {
      x_ += dx_ * gridPerSec_ * renderTimeInSec;
      item_->setPos(x_, y_);
   }

   if (dy_ != 0.0)
   {
      y_ += dy_ * gridPerSec_ * renderTimeInSec;
      item_->setPos(x_, y_);
   }

//qDebug() << x_ << y_;

   int32_t x = (int32_t)floor(x_);
   int32_t y = (int32_t)floor(y_);

   if ((x != lastX_) || (y != lastY_))
   {
qDebug() << x << y << map_;
      lastX_ = x;
      lastY_ = y;

      //uint32_t lastTile = map_->GetTile(lastX_, lastY_) - 1;

      struct Dir
      {
         int32_t dx;
         int32_t dy;
      };

      uint32_t minDir = 4;
      uint32_t minEgg = 999;

      const Dir dir[4] = {{ 0,-1},
                          { 1, 0},
                          { 0, 1},
                          {-1, 0}};

      for (uint32_t testDir = 0; testDir < 4; ++testDir)
      {
         int32_t testX = lastX_ + dir[testDir].dx;
         int32_t testY = lastY_ + dir[testDir].dy;

qDebug() << testX << testY;

         if (map_->InBounds(testX, testY) == true)
         {
qDebug() << "OK";

            uint32_t testEgg  = map_->GetEgg(testX, testY);

            if (testEgg < minEgg)
            {
               minDir = testDir;
               minEgg = testEgg;
            }
         }
      }

      SetVel(dir[minDir].dx, dir[minDir].dy);

   }

   item_->setZValue(y_); // Use y value for z-sorting.
}
