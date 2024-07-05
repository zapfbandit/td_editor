#include "tilestore.h"

#include <QDir>
#include <QMessageBox>


TileStore::TileStore(const QString& tilesDirPath):
   tilesDirPath_(tilesDirPath)
{
   QDir tilesDir(tilesDirPath);
   tilesDir.setFilter( QDir::Files | QDir::NoDotAndDotDot );
   QFileInfoList tilesList = tilesDir.entryInfoList();

   numTiles_ = tilesDir.count();

//qDebug() << numTiles_;

   tiles_ = new Tile[numTiles_];
   tileNameMap_ = new QString[numTiles_];
   for (uint32_t i = 0; i < numTiles_; ++i)
   {
      QFileInfo fileInfo = tilesList.at(i);

//qDebug() << fileInfo.absoluteFilePath();
qDebug() << tilesDir.relativeFilePath(fileInfo.absoluteFilePath()) << i;

      tiles_[i].LoadImagePath(fileInfo.absoluteFilePath());
      tileNameMap_[i] = tilesDir.relativeFilePath(fileInfo.absoluteFilePath());
   }
}




TileStore::~TileStore()
{
   delete [] tiles_;
}


QString TileStore::TilesDirPath() const
{
   return tilesDirPath_;
}


uint32_t TileStore::NumTiles()
{
   return numTiles_;
}


Tile& TileStore::GetTile(uint32_t i)
{
   if (i >= numTiles_)
   {
      qDebug() << "TileStore::GetTile Killed us all";
      exit(EXIT_FAILURE);
   }
   
   return tiles_[i];
}


uint32_t TileStore::GetTileIndex(const QString& relFilePath)
{
   for (uint32_t i = 0; i < numTiles_; ++i)
   {
      if (tileNameMap_[i] == relFilePath)
      {
         return i;
      }
   }

   qDebug() << "Unable to find image" << relFilePath << "exiting...";

   QMessageBox::critical(nullptr,
                         QString("TD Editor"),
                         QString("Unable to find image \"%0\".").arg(relFilePath));

   exit(EXIT_FAILURE);

   return numTiles_;
}
