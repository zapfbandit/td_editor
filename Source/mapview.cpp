#include "mapview.h"

#include <QMouseEvent>

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCheckBox>
#include <QGraphicsLineItem>


MapView::MapView(QWidget* parent):
   QGraphicsView(parent),
   tileStore_(nullptr),
   width_(0),
   height_(0),
   map_(nullptr),
   egg_(nullptr),
   showGrid_(true)
{
   setScene(&scene_);
}


MapView::~MapView()
{
   delete [] map_;
   delete [] egg_;
   delete grid_;
}


void MapView::ShowGrid(const bool showGrid)
{
   showGrid_ = showGrid;

   Render();
}


void MapView::SetSettings(Settings* settings)
{
   settings_ = settings;
}


void MapView::SetTileStore(TileStore* tileStore)
{
   tileStore_ = tileStore;
}


void MapView::SetSelectedView(SelectedView* selectedView)
{
   selectedView_ = selectedView;
}


void MapView::SetSize(const uint32_t width, const uint32_t height)
{
   scene_.clear();

   width_  = width;
   height_ = height;

   delete [] map_;
   delete [] egg_;

   map_ = new uint32_t[width * height];
   egg_ = new uint32_t[width * height];

   for (uint32_t y = 0; y < height_; ++y)
   {
      for (uint32_t x = 0; x < width_; ++x)
      {
         map_[width_ * y + x] = 0;
         egg_[width_ * y + x] = 999; // width * height;
      }
   }

   grid_ = new QGraphicsItemGroup;

   for (uint32_t y = 0; y <= height_; ++y)
   {
      QGraphicsLineItem* line = scene_.addLine(0.0, TILE_SIZE * y, TILE_SIZE * width_, TILE_SIZE * y);
      QPen pen = line->pen();
      pen.setWidthF(0.025);
      line->setPen(pen);
      grid_->addToGroup(line);
   }

   for (uint32_t x = 0; x <= width_; ++x)
   {
      QGraphicsLineItem* line = scene_.addLine(TILE_SIZE * x, 0.0, TILE_SIZE * x, TILE_SIZE * height_);
      QPen pen = line->pen();
      pen.setWidthF(0.025);
      line->setPen(pen);
      grid_->addToGroup(line);
   }

   grid_->setZValue(-65);
   scene_.addItem(grid_);

   Render();
}


uint32_t MapView::GetTile(const uint32_t x,
                          const uint32_t y) const
{
   return map_[width_ * y + x];
}


uint32_t MapView::GetEgg(const uint32_t x,
                         const uint32_t y) const
{
//qDebug() << "MapView::GetEgg" << x << y << (width_ * y + x);
   return egg_[width_ * y + x];
}

bool MapView::InBounds(const int32_t x,
                       const int32_t y) const
{
   return (x >= 0) && (y >= 0) && (x < width_) && (y < height_);
}


void MapView::Render()
{
   for (uint32_t y = 0; y < height_; ++y)
   {
      for (uint32_t x = 0; x < width_; ++x)
      {
         tileStore_->GetTile(map_[width_ * y + x]).Render(scene_, TILE_SIZE * x, TILE_SIZE * y, TILE_SIZE);
      }
   }

   if (grid_ != nullptr)
   {
      grid_->setVisible(showGrid_);
   }

   resizeEvent(nullptr);
}


void MapView::resizeEvent(QResizeEvent *event)
{
   QRectF sceneRect(0.0, 0.0, TILE_SIZE * width_, TILE_SIZE * height_);

   fitInView(sceneRect, Qt::KeepAspectRatio);
   
   QGraphicsView::resizeEvent(event);
}


void MapView::mousePressEvent(QMouseEvent* event)
{
   QPointF sceneClick = mapToScene(event->pos());

   uint32_t x = static_cast<uint32_t>(sceneClick.x() / TILE_SIZE);
   uint32_t y = static_cast<uint32_t>(sceneClick.y() / TILE_SIZE);
   uint32_t oldTile = map_[width_ * y + x];
   uint32_t newTile = 0;

   if (event->button() == Qt::LeftButton)
   {
       newTile = selectedView_->GetSelected();
   }
   else
   {
      selectedView_->SetSelected(map_[width_ * y + x]);
      newTile = 0;
   }

   DoChange(x, y, newTile);

   emit Changed(x, y, oldTile, newTile);

   QGraphicsView::mousePressEvent(event);
}


uint32_t MapView::NumSpawns()
{
   return spawns_.size();
}


MapView::Spawn& MapView::GetSpawn(const uint32_t num)
{
   return spawns_[num];
}


bool MapView::OpenFile(const QString& path)
{
   QFile file(path);
   if (file.open(QIODevice::ReadOnly))
   {
      QTextStream stream(&file);

      bool indexChanged = false;

      std::map<uint32_t, uint32_t> indexMap;

      QString readStr;

      stream >> readStr;
//qDebug() << readStr;
      if (readStr != "Tiles")
      {
         qDebug() << "MapView::OpenFile Killed us all";
         exit(EXIT_FAILURE);
      }

      uint32_t numTiles;
      stream >> numTiles;

//qDebug() << numTiles;

      uint32_t index;
      QString relFilePath;
      for (uint32_t i = 0; i < numTiles; ++i)
      {
         stream >> index >> relFilePath;
         uint32_t newIndex = tileStore_->GetTileIndex(relFilePath);
         indexMap[index] = newIndex;
//qDebug() << index << relFilePath << newIndex;
         if (index != newIndex)
         {
            indexChanged = true;
         }
      }

      stream >> readStr;
//qDebug() << readStr;
      if (readStr != "Map")
      {
         qDebug() << "MapView::OpenFile Killed us all";
         exit(EXIT_FAILURE);
      }

      uint32_t width;
      uint32_t height;

      stream >> width >> height;
//qDebug() << width << height;

      SetSize(width, height);

      struct EggPos
      {
         uint32_t x;
         uint32_t y;
      };

      std::vector<EggPos> eggPos_;
      for (uint32_t y = 0; y < height_; ++y)
      {
         for (uint32_t x = 0; x < width_; ++x)
         {
            uint32_t tileId;
            stream >> tileId;
            map_[width_ * y + x] = indexMap[tileId];
            uint32_t testGrid = indexMap[tileId] - 1;
            if ((testGrid == 0x7) ||
                (testGrid == 0xB) ||
                (testGrid == 0xD) ||
                (testGrid == 0xE))
            {
                qDebug() << testGrid << x << y;
                eggPos_.push_back({x, y});
            }
         }
      }

for (auto it: eggPos_)
{
   qDebug() << "Egg" << it.x << it.y;
   DoEggFlood(it.x, it.y, 100);
}

for (uint32_t y = 0; y < height_; ++y)
{
   QDebug debug = qDebug();
   for (uint32_t x = 0; x < width_; ++x)
   {
      debug << egg_[width_ * y + x] << " ";
   }
   //stream << "\r\n";
}

      stream >> readStr;
//qDebug() << readStr;
      if (readStr != "Spawns")
      {
         qDebug() << "MapView::OpenFile Killed us all";
         exit(EXIT_FAILURE);
      }

      spawns_.clear();

      uint32_t numSpawns = 0;
      stream >> numSpawns;

      for (uint32_t i = 0; i < numSpawns; ++i)
      {
         uint32_t index;
         double x, y;
         double dx, dy;
         stream >> index >> x >> y >> dx >> dy;
         spawns_.push_back({index, x, y, dx, dy});
      }

      uint32_t i = 0;
      for (auto it: spawns_)
      {
qDebug() << it.index_ << "(" << it.x_ << it.y_ << ") (" << it.dx_ << it.dy_ <<  ")";
      }

      file.close();

      Render();

      if (indexChanged == true)
      {
         if (settings_->IgnoreIndexWarning() == false)
         {
            QMessageBox indexWarningMsgBox;
            indexWarningMsgBox.setText(QString("The tile index for \"%0\" was changed.\n"
                              "The map will be marked as modified.").arg(QFileInfo(path).fileName()));
            indexWarningMsgBox.setIcon(QMessageBox::Icon::Warning);
            indexWarningMsgBox.addButton(QMessageBox::Ok);
            QCheckBox *cb = new QCheckBox("Don't show this again", &indexWarningMsgBox);
            indexWarningMsgBox.setCheckBox(cb);

            QObject::connect(cb, &QCheckBox::stateChanged, this, [this](int state)
            {
               settings_->IgnoreIndexWarning(state == Qt::CheckState::Checked);
            });

            indexWarningMsgBox.exec();
         }

         emit Touched();
      }

      return true;
   }

   return false;
}


void MapView::DoEggFlood(const uint32_t x,
                         const uint32_t y,
                         const uint32_t dist)
{
   egg_[y * width_ + x] = dist;

   uint32_t tileId = map_[width_ * y + x] - 1;

   if ((y >= 1)          && ((tileId & 0x1) == 0) && (((map_[width_ * (y - 1) + (x + 0)] - 1) & 0x4) == 0) && (egg_[(y - 1) * width_ + (x + 0)] > dist)) DoEggFlood(x + 0, y - 1, dist + 1);
   if ((x < width_ - 1)  && ((tileId & 0x2) == 0) && (((map_[width_ * (y + 0) + (x + 1)] - 1) & 0x8) == 0) && (egg_[(y + 0) * width_ + (x + 1)] > dist)) DoEggFlood(x + 1, y + 0, dist + 1);
   if ((y < height_ - 1) && ((tileId & 0x4) == 0) && (((map_[width_ * (y + 1) + (x + 0)] - 1) & 0x1) == 0) && (egg_[(y + 1) * width_ + (x + 0)] > dist)) DoEggFlood(x + 0, y + 1, dist + 1);
   if ((x >= 1)          && ((tileId & 0x8) == 0) && (((map_[width_ * (y + 0) + (x - 1)] - 1) & 0x2) == 0) && (egg_[(y + 0) * width_ + (x - 1)] > dist)) DoEggFlood(x - 1, y + 0, dist + 1);
}


bool MapView::SaveToFile(const QString& path)
{
   QFile file(path);
   if (file.open(QIODevice::WriteOnly))
   {
      QTextStream stream(&file);

      QDir dir(tileStore_->TilesDirPath());

      std::map<uint32_t, QString> indexMap;

      for (uint32_t y = 0; y < height_; ++y)
      {
         for (uint32_t x = 0; x < width_; ++x)
         {
            uint32_t tileId = map_[width_ * y + x];
            indexMap[ tileId ] = dir.relativeFilePath(tileStore_->GetTile(tileId).GetImagePath());
         }
      }

      stream << qSetFieldWidth(0) << Qt::left;
      stream << "Tiles" << "\r\n";
      stream << qSetFieldWidth(4) << Qt::right;

      stream << indexMap.size() << "\r\n";

      for (auto& it: indexMap)
      {
         stream << it.first << "" << it.second << "\r\n";
      }

      stream << qSetFieldWidth(0) << Qt::left;
      stream << "Map" << "\r\n";
      stream << qSetFieldWidth(4) << Qt::right;

      stream << width_ << height_ << "\r\n";

      for (uint32_t y = 0; y < height_; ++y)
      {
         for (uint32_t x = 0; x < width_; ++x)
         {
            stream << map_[width_ * y + x];
         }
         stream << "\r\n";
      }

      stream << qSetFieldWidth(0) << Qt::left;
      stream << "Spawns" << "\r\n";
      stream << qSetFieldWidth(4) << Qt::right;

      spawns_.clear();
      uint32_t index = 0;
      uint32_t tileId = 0;

      for (uint32_t y = 0; y < height_; ++y)
      {
         tileId = map_[width_ * (y + 0) + (0)] - 1;
         if ((tileId != 0) && ((tileId & 0x8) == 0))
         {
            spawns_.push_back({index++, 0, (double)y, -1, 0});
         }

         tileId = map_[width_ * (y + 0) + (width_ - 1)] - 1;
         if ((tileId != 0) && ((tileId & 0x2) == 0))
         {
            spawns_.push_back({index++, (double)(width_ - 1), (double)y, 1, 0});
         }
      }

      for (uint32_t x = 0; x < width_; ++x)
      {
         tileId = map_[width_ * (0) + x] - 1;
         if ((tileId != 0) && ((tileId & 0x1) == 0))
         {
            spawns_.push_back({index++, (double)x, 0, 0, -1});
         }

         tileId = map_[width_ * (height_ - 1) + x] - 1;
         if ((tileId != 0) && ((tileId & 0x4) == 0))
         {
            spawns_.push_back({index++, (double)x, (double)(height_ - 1), 0, 1});
         }
      }

      stream << spawns_.size() << "\r\n";

      uint32_t i = 0;
      for (auto it: spawns_)
      {
         stream << it.index_ << it.x_ << it.y_ << it.dx_ << it.dy_ << "\r\n";
      }

      file.close();

      return true;
   }

   return false;
}


void MapView::DoChange(const uint32_t x,
                       const uint32_t y,
                       const uint32_t tile)
{
   map_[width_ * y + x] = tile;

   Render();
}
