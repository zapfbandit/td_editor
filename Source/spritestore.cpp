#include "spritestore.h"

#include <QDir>
#include <QMessageBox>


SpriteStore::SpriteStore(const QString& spritesDirPath):
   spritesDirPath_(spritesDirPath),
   spritesDir_(spritesDirPath),
   numSprites_(0),
   offset_(0)
{
   GetNumSprites(spritesDirPath);

qDebug() << numSprites_;

   sprites_ = new Sprite[numSprites_];
   spriteNameMap_ = new QString[numSprites_];

   LoadSprites(spritesDirPath);

   if (offset_ != numSprites_)
   {
      qDebug() << "Something bad happened";
   }
}


void SpriteStore::GetNumSprites(const QString& dirPath)
{
   QDir currDir(dirPath);

   QFileInfoList spritesList = currDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
   numSprites_ += spritesList.count();

   QFileInfoList dirList = currDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

   for (QFileInfo& dir: dirList)
   {
      GetNumSprites(dir.absoluteFilePath());
   }
}


void SpriteStore::LoadSprites(const QString& dirPath)
{
   QDir currDir(dirPath);

   QFileInfoList spritesList = currDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

   for (QFileInfo& dir: spritesList)
   {
      qDebug() << spritesDir_.relativeFilePath(dir.absoluteFilePath()) << offset_;

      sprites_[offset_].LoadImagePath(dir.absoluteFilePath());
      spriteNameMap_[offset_] = spritesDir_.relativeFilePath(dir.absoluteFilePath());

      offset_++;
   }

   QFileInfoList dirList = currDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

   for (QFileInfo& dir: dirList)
   {
      LoadSprites(dir.absoluteFilePath());
   }
}


SpriteStore::~SpriteStore()
{
   delete [] sprites_;
   delete [] spriteNameMap_;
}


QString SpriteStore::SpritesDirPath() const
{
   return spritesDirPath_;
}


uint32_t SpriteStore::NumSprites()
{
   return numSprites_;
}


Sprite& SpriteStore::GetSprite(uint32_t i)
{
   if (i >= numSprites_)
   {
      exit(EXIT_FAILURE);
   }
   
   return sprites_[i];
}


uint32_t SpriteStore::GetSpriteIndex(const QString& relFilePath)
{
   for (uint32_t i = 0; i < numSprites_; ++i)
   {
      if (spriteNameMap_[i] == relFilePath)
      {
         return i;
      }
   }

   qDebug() << "Unable to find image" << relFilePath << "exiting...";

   QMessageBox::critical(nullptr,
                         QString("TD Editor"),
                         QString("Unable to find image \"%0\".").arg(relFilePath));

   exit(EXIT_FAILURE);

   return numSprites_;
}
