#pragma once

#include <QGraphicsView>

#include <QGraphicsItemGroup>

#include "settings.h"
#include "tilestore.h"
#include "selectedview.h"
#include "spawndelegate.h"
#include "spawnmgr.h"


class MapView : public QGraphicsView
{
   Q_OBJECT

public:

   MapView(QWidget* parent);
   ~MapView();

public:

   void ShowGrid(const bool showGrid);

   void SetSettings(Settings* settings);
   void SetTileStore(TileStore* tileStore);
   void SetSelectedView(SelectedView* selectedView);
   void SetSpawnMgr(SpawnMgr* spawnMgr);
   void SetDelegate(SpawnDelegate* spawnDelegate);
   void SetSize(const uint32_t width, const uint32_t height);
   void Render();

public:
   uint32_t Width() const;
   uint32_t Height() const;
   uint32_t GetTile(const uint32_t x, const uint32_t y) const;
   uint32_t GetEgg (const uint32_t x, const uint32_t y) const;
   bool InBounds   (const int32_t  x, const int32_t  y) const;


public:

   bool OpenFile(const QString& path);
   bool SaveToFile(const QString& path);

public:
   void DoEggFlood(const uint32_t x,
                   const uint32_t y,
                   const uint32_t dist);

   void DoChange(const uint32_t x,
                 const uint32_t y,
                 const uint32_t tile);

signals:
   void Touched();
   void Changed(uint32_t x, uint32_t y, uint32_t oldTile, uint32_t newTile);

protected:

   virtual void resizeEvent(QResizeEvent *event) override;
   virtual void mousePressEvent(QMouseEvent *event) override;
   virtual void wheelEvent(QWheelEvent* event) override;

private:

   QGraphicsScene scene_;

   Settings* settings_{nullptr};

   TileStore* tileStore_{nullptr};

   SelectedView* selectedView_{nullptr};

   uint32_t width_;
   uint32_t height_;

   uint32_t* map_;
   uint32_t* egg_;

   bool showGrid_;
   QGraphicsItemGroup* gridGroup_;
   QGraphicsItemGroup* mapGroup_;

   SpawnDelegate* spawnDelegate_{nullptr};
   SpawnMgr* spawnMgr_{nullptr};

   static constexpr double TILE_SIZE = 1.0;
};

