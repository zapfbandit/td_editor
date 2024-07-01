#pragma once

#include <QGraphicsScene>


class Tile
{
public:

   Tile();

   Tile(const QString &imagePath);

public:

   void LoadImagePath(const QString& imagePath);

   QString GetImagePath() const;

   QGraphicsPixmapItem* Render(QGraphicsScene& scene,
                               const qreal x,
                               const qreal y,
                               const qreal size);

private:

   QString imagePath_;
   QPixmap pixmap_;
};

