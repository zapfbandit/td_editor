#pragma once

#include <QGraphicsView>

#include "tilestore.h"
#include "selectedview.h"


class TilesView : public QGraphicsView
{
   Q_OBJECT

public:

   TilesView(QWidget* parent);

public:

   void SetTileStore(TileStore* tileStore);
   void SetSelectedView(SelectedView* selectedView);
   void Render();

protected:

   virtual void resizeEvent(QResizeEvent *event) override;
   virtual void mousePressEvent(QMouseEvent *event) override;
   
private:

   QGraphicsScene scene_;

   TileStore* tileStore_{nullptr};

   SelectedView* selectedView_;

   static constexpr double TILE_SIZE = 1.0;
   static constexpr double TILE_SPACE = 1.0 / 64.0;
};

