#pragma once

#include <stdint.h>

#include <QPixmap>

#include <QString>

#include <QDir>


class PixmapStore
{
public:

   PixmapStore(const QString& pixmapDirPath);
   ~PixmapStore();


public:

   struct ItemInfo
   {
      const uint32_t firstAnimFrameIndex_;
      const uint32_t numAnimFrames_;
   };

public:

   void CalcNumPixmaps(const QString& dirPath);
   void LoadPixmaps(const QString& dirPath);

public:

   uint32_t NumPixmaps() const;
   uint32_t GetPixmapIndex(QString pixmapRelPath);
   QPixmap* GetPixmap(uint32_t i);

private:

   QDir pixmapDir_;

   uint32_t numPixmaps_;
   uint32_t offset_;

   QPixmap* pixmap_;
   QString* pixmapRelPath_;
};

