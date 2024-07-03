#pragma once

#include <QGraphicsView>

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
   uint32_t NumSpawns();
   Spawn& GetSpawn(const uint32_t num);

public:

   bool OpenFile(const QString& path);
   bool SaveToFile(const QString& path);

signals:
   void Changed();

protected:

   virtual void resizeEvent(QResizeEvent *event) override;
   virtual void mousePressEvent(QMouseEvent *event) override;
   
private:

   QGraphicsScene scene_;

   Settings* settings_{nullptr};

   TileStore* tileStore_{nullptr};

   SelectedView* selectedView_;

   uint32_t width_;
   uint32_t height_;

   uint32_t* map_;

   bool showGrid_;

   std::vector<Spawn> spawns_;

   static constexpr double TILE_SIZE = 1.0;
};

