#include <QGraphicsPixmapItem>

#include "tile.h"



Tile::Tile()
{}


Tile::Tile(const QString& imagePath)
{
   LoadImagePath(imagePath);
}



void Tile::LoadImagePath(const QString& imagePath)
{
   imagePath_ = imagePath;
   pixmap_ = QPixmap(imagePath);
   if (pixmap_.width() != pixmap_.height())
   {
      exit(EXIT_FAILURE);
   }
}



QString Tile::GetImagePath() const
{
   return imagePath_;
}


QGraphicsPixmapItem* Tile::Render(QGraphicsScene& scene,
                                  const qreal x,
                                  const qreal y,
                                  const qreal size)
{
   QGraphicsPixmapItem* item = scene.addPixmap(pixmap_);

   item->setPos(x, y);
   item->setScale(size / pixmap_.width());
   item->setZValue(-66);

   return item;
}
