#include "pixmapstore.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>


PixmapStore::PixmapStore(const QString& pixmapDirPath):
   pixmapDir_(pixmapDirPath),
   numPixmaps_(0),
   offset_(0)
{
   CalcNumPixmaps(pixmapDirPath);

   pixmap_ = new QPixmap[numPixmaps_];
   pixmapRelPath_ = new QString[numPixmaps_];

   LoadPixmaps(pixmapDirPath);

   if (offset_ != numPixmaps_)
   {
      qDebug() << "Something bad happened";
   }
}


void PixmapStore::CalcNumPixmaps(const QString& dirPath)
{
qDebug() << QString("PixmapStore::CalcNumPixmaps(dirPath = %0)").arg(dirPath);

   QDir currDir(dirPath);

   QFileInfoList pixmapList = currDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
   numPixmaps_ += pixmapList.count();

   QFileInfoList dirList = currDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

   for (QFileInfo& dir: dirList)
   {
      CalcNumPixmaps(dir.absoluteFilePath());
   }
}


void PixmapStore::LoadPixmaps(const QString& dirPath)
{
qDebug() << QString("PixmapStore::LoadPixmaps = %0").arg(dirPath);

   QDir currDir(dirPath);

   QFileInfoList pixmapList = currDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);   
   uint32_t numAnimFrames = pixmapList.size();

   if (numAnimFrames > 0)
   {
      for (QFileInfo& dir: pixmapList)
      {
         pixmap_[offset_] = QPixmap(dir.absoluteFilePath());

         QString animDirPath = pixmapDir_.relativeFilePath(pixmapList[0].absolutePath());

         pixmapRelPath_[offset_] = animDirPath;

qDebug() << animDirPath << numAnimFrames;

         offset_++;
      }
   }

   QFileInfoList dirList = currDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

   for (QFileInfo& dir: dirList)
   {
      LoadPixmaps(dir.absoluteFilePath());
   }
}


PixmapStore::~PixmapStore()
{
qDebug() << "PixmapStore::Its a killin' time...";

   delete [] pixmap_;
}


uint32_t PixmapStore::NumPixmaps() const
{
qDebug() << QString("PixmapStore::NumPixmaps() = %0").arg(numPixmaps_);

   return numPixmaps_;
}


QPixmap* PixmapStore::GetPixmap(const uint32_t i)
{
//qDebug() << QString("PixmapStore::GetPixmap(i = %0)").arg(i);

   if (i >= numPixmaps_)
   {
      qDebug() << "PixmapStore::GetPixmap Killed us all";
      exit(EXIT_FAILURE);
   }

   return &pixmap_[i];
}


uint32_t PixmapStore::GetPixmapIndex(QString pixmapRelPath)
{
qDebug() << QString("PixmapStore::GetPixmap(name = %0; numPixmaps_ = %1)").arg(pixmapRelPath).arg(numPixmaps_);

   for (uint32_t i = 0; i < numPixmaps_; ++i)
   {
      if (pixmapRelPath_[i] == pixmapRelPath)
      {
         return i;
      }
   }

   qDebug() << "Unable to find image" << pixmapRelPath << "exiting...";

   QMessageBox::critical(nullptr,
                         QString("TD Editor"),
                         QString("Unable to find image \"%0\".").arg(pixmapRelPath));

   exit(EXIT_FAILURE);

   return numPixmaps_;
}

