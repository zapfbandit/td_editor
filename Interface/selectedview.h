#pragma once

#include <QGraphicsView>

#include <QGraphicsScene>

#include "tilestore.h"


class SelectedView : public QGraphicsView
{
   Q_OBJECT

public:

   SelectedView(QWidget* parent);

public:

   void SetTileStore(TileStore* tileStore);
   void SetSelected(const uint32_t sel);
   uint32_t GetSelected() const;
   void Render();

protected:

   virtual void resizeEvent(QResizeEvent *event) override;
   
private:

   QGraphicsScene scene_;
   
   TileStore* tileStore_{nullptr};

   uint32_t sel_;

   static constexpr double TILE_SIZE = 1.0;
};

