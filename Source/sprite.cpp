#include "sprite.h"

#include <QGraphicsScene>

#include "spritemgr.h"
#include "pixmapstore.h"


/*static*/ bool Sprite::s_inited_ = false;
/*static*/ SpriteMgr* Sprite::s_mgr_ = nullptr;
/*static*/ QGraphicsScene* Sprite::s_scene_ = nullptr;
/*static*/ PixmapStore* Sprite::s_store_ = nullptr;
/*static*/ MapView* Sprite::s_map_ = nullptr;


Sprite::Sprite(const double   x,
               const double   y,
               const double   dx,
               const double   dy,
               const double   scale,

               const QString& spriteType,
               const QString& spriteName,
               const uint32_t numFrames,
               const double   framesPerSec,
               const double   gridPerSec):
   item_(nullptr)
{
   qDebug() << QString("Sprite::Sprite(...)");

   frame_ = 0;
   dist_ = 0.0;

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

   QPixmap& pixmap = *s_store_->GetPixmap(baseFrameIndex_);

   // Centre the sprite

   SetPos(x - 0.5 * scale,
          y - 0.5 * scale);

//qDebug() << "Pos" << x << y;

   item_ = s_scene_->addPixmap(pixmap);
   item_->setScale(scale / pixmap.width());
   item_->setPos(x, y);

   qDebug() << item_;

   Tick(0.0);}


Sprite::~Sprite()
{}



/*static*/ void Sprite::Init(SpriteMgr*      mgr,
                             QGraphicsScene* scene,
                             PixmapStore*    store,
                             MapView*        map)
{
   s_mgr_ = mgr;
   s_scene_ = scene;
   s_store_ = store;
   s_map_   = map;
   s_inited_ = true;
}


bool Sprite::Destroy()
{
//qDebug() << "Sprite::Destroy()";
   s_scene_->removeItem(item_);
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

   baseFrameIndex_ = s_store_->GetPixmapIndex(QString("%0/%1/Walk/%2").
                                             arg(spriteType_,
                                                 spriteName_,
                                                 spriteDir_));
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
      item_->setPixmap(*s_store_->GetPixmap(frame_));
   }

//qDebug() << "scale_ = " << scale_ << "item_->scale() = " << item_->scale();

   if (dx_ != 0.0)
   {
      x_ += dx_ * gridPerSec_ * renderTimeInSec;
      item_->setPos(x_, y_);
      dist_ += gridPerSec_ * renderTimeInSec;
   }

   if (dy_ != 0.0)
   {
      y_ += dy_ * gridPerSec_ * renderTimeInSec;
      item_->setPos(x_, y_);
      dist_ += gridPerSec_ * renderTimeInSec;
   }

   if (dist_ >= 1.0)
   {
      dist_ -= 1.0;

      int32_t gx = round(x_);
      int32_t gy = round(y_);

qDebug() << "DoIt: " << x_ << y_ << gx << gy;

      if (s_map_->InBounds(gx, gy) == false)
      {
         // Something really really bad... maybe a counter
         qDebug() << "BAD !!!";
      }
      else
      {
         uint32_t currTile = s_map_->GetTile(gx, gy);
         uint32_t currEgg  = s_map_->GetEgg (gx, gy);

         struct Dir
         {
            int32_t dx;
            int32_t dy;
            int32_t out;
            int32_t in;
         };

         uint32_t matchDir = 4;

         const Dir dir[4] = {{ 0,-1, 1, 4},
                             { 1, 0, 2, 8},
                             { 0, 1, 4, 1},
                             {-1, 0, 8, 2}};

         for (uint32_t testDir = 0; (matchDir == 4) && (testDir < 4); ++testDir)
         {
            int32_t testX = gx + dir[testDir].dx;
            int32_t testY = gy + dir[testDir].dy;

            if (s_map_->InBounds(testX, testY) == true)
            {
               uint32_t testTile = s_map_->GetTile(testX, testY);
               uint32_t testEgg  = s_map_->GetEgg (testX, testY);

               if ((((currTile - 1) & dir[testDir].out) == 0) &&
                   (((testTile - 1) & dir[testDir].in)  == 0) &&
                   (currEgg == testEgg + 1))
               {
                  matchDir = testDir;
               }
            }
         }

         if (matchDir != 4)
         {
            SetVel(dir[matchDir].dx, dir[matchDir].dy);
         }
         else
         {
qDebug() << "Killing sprite...";

            s_mgr_->MarkForDeath(this);

            // Egg Health -= Baddy Heath
         }
      }
   }

   item_->setZValue(y_); // Use y value for z-sorting.
}
