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

   void ShowGrid(const bool showGrid);

   void SetSettings(Settings* settings);
   void SetTileStore(TileStore* tileStore);
   void SetSelectedView(SelectedView* selectedView);
   void SetSize(const uint32_t width, const uint32_t height);
   void Render();

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

   struct Spawn
   {
      uint32_t x_;
      uint32_t y_;
      int32_t dx_;
      int32_t dy_;
   };

   std::vector<Spawn> spawns_;

   static constexpr double TILE_SIZE = 1.0;
};

