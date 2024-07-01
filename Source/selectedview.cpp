#include "selectedview.h"


SelectedView::SelectedView(QWidget* parent):
   QGraphicsView(parent),
   tileStore_(nullptr),
   sel_(0)
{
   setScene(&scene_);
}


void SelectedView::SetTileStore(TileStore* tileStore)
{
   tileStore_ = tileStore;

   SetSelected(0);
}


void SelectedView::SetSelected(const uint32_t sel)
{
   sel_ = sel;

   Render();
}


uint32_t SelectedView::GetSelected() const
{
   return sel_;
}


void SelectedView::Render()
{
   scene_.clear();

   tileStore_->GetTile(sel_).Render(scene_, 0.0, 0.0, TILE_SIZE);

   resizeEvent(nullptr);
}


void SelectedView::resizeEvent(QResizeEvent *event)
{
   QRectF sceneRect(0.0, 0.0, TILE_SIZE, TILE_SIZE);

   fitInView(sceneRect, Qt::KeepAspectRatio);
   
   QGraphicsView::resizeEvent(event);
}
