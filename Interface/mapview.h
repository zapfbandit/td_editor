#pragma once

#include <QGraphicsView>

#include <QGraphicsItemGroup>

#include "settings.h"
#include "tilestore.h"
#include "selectedview.h"


class MapView : public QGraphicsView
{
   Q_OBJECT

public:

   MapView(QWidget* parent);
   ~MapView();

public:

   struct Spawn
   {
      uint32_t index_;
      double x_;
      double y_;
      double dx_;
      double dy_;
   };

public:

   void ShowGrid(const bool showGrid);

   void SetSettings(Settings* settings);
   void SetTileStore(TileStore* tileStore);
   void SetSelectedView(SelectedView* selectedView);
   void SetSize(const uint32_t width, const uint32_t height);
   void Render();

public:
   uint32_t GetTile(const uint32_t x, const uint32_t y) const;
   uint32_t GetEgg (const uint32_t x, const uint32_t y) const;
   bool InBounds   (const int32_t  x, const int32_t  y) const;

public:
   uint32_t NumSpawns();
   Spawn& GetSpawn(const uint32_t num);

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

   SelectedView* selectedView_;

   uint32_t width_;
   uint32_t height_;

   uint32_t* map_;
   uint32_t* egg_;

   bool showGrid_;
   QGraphicsItemGroup* gridGroup_;
   QGraphicsItemGroup* mapGroup_;

   std::vector<Spawn> spawns_;

   static constexpr double TILE_SIZE = 1.0;
};

