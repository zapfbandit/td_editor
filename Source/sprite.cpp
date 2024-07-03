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
                  PixmapStore*    store)
{
   scene_ = scene;
   store_ = store;
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

   baseFrameIndex_ = store_->GetPixmapIndex(QString("%0/%1/Walk/%2").
                                             arg(spriteType_).
                                             arg(spriteName_).
                                             arg(spriteDir_));

   QPixmap& pixmap = *store_->GetPixmap(baseFrameIndex_);

   // Centre the sprite
   SetPos(x - 0.5 * scale, y - 0.5 * scale);

//qDebug() << "Pos" << x << y;

   item_ = scene_->addPixmap(pixmap);
   item_->setScale(scale / pixmap.width());
   item_->setPos(x, y);
   item_->setZValue(1.0); // Move to front

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
}


void Sprite::UpdateFrame(const uint32_t frame)
{
   item_->setPixmap(*store_->GetPixmap(frame));
}


void Sprite::UpdatePos(const double x,
                       const double y)
{
   item_->setPos(x, y);
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
      UpdateFrame(frame_);
   }

//qDebug() << "scale_ = " << scale_ << "item_->scale() = " << item_->scale();

   if (dx_ != 0.0)
   {
      x_ += dx_ * gridPerSec_ * renderTimeInSec;
      UpdatePos(x_, y_);
   }

   if (dy_ != 0.0)
   {
      y_ += dy_ * gridPerSec_ * renderTimeInSec;
      UpdatePos(x_, y_);
   }

//qDebug() << x_ << y_;
}
