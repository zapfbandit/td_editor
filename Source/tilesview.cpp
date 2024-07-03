#include "tilesview.h"

#include <QMouseEvent>


TilesView::TilesView(QWidget* parent):
   QGraphicsView(parent),
   tileStore_(nullptr)
{
   setScene(&scene_);
}


void TilesView::SetTileStore(TileStore* tileStore)
{
   tileStore_ = tileStore;

   Render();
}


void TilesView::SetSelectedView(SelectedView* selectedView)
{
   selectedView_ = selectedView;
}


void TilesView::Render()
{
   scene_.clear();

   for (uint32_t i = 0; i < tileStore_->NumTiles(); ++i)
   {
      tileStore_->GetTile(i).Render(scene_,
                                    TILE_SPACE / 2.0 ,
                                    TILE_SPACE + (TILE_SIZE + TILE_SPACE) * i,
                                    TILE_SIZE);
   }

   resizeEvent(nullptr);
}


void TilesView::resizeEvent(QResizeEvent *event)
{
   double ratio = static_cast<double>(height()) / width();

   QRectF sceneRect(0.0, 0.0, TILE_SIZE + TILE_SPACE, (TILE_SIZE + TILE_SPACE) * ratio);

   fitInView(sceneRect, Qt::KeepAspectRatio);
   
   QGraphicsView::resizeEvent(event);
}


void TilesView::mousePressEvent(QMouseEvent *event)
{
   QPointF sceneClick = mapToScene(event->pos());

   uint32_t tileIndex = static_cast<uint32_t>(sceneClick.y() / (TILE_SIZE + TILE_SPACE));
   
//qDebug() << tileIndex;

   selectedView_->SetSelected(tileIndex);

   QGraphicsView::mousePressEvent(event);
}
