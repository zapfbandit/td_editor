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
   showGrid_(true)
{
   setScene(&scene_);
}


MapView::~MapView()
{
   delete [] map_;
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
   width_  = width;
   height_ = height;

   delete [] map_;

   map_ = new uint32_t[width * height];

   for (uint32_t y = 0; y < height_; ++y)
   {
      for (uint32_t x = 0; x < width_; ++x)
      {
         map_[width_ * y + x] = 0;
      }
   }

   Render();
}


void MapView::Render()
{
   //scene_.clear();

//qDebug() << width_ << "," << height_;

   for (uint32_t y = 0; y < height_; ++y)
   {
      for (uint32_t x = 0; x < width_; ++x)
      {
//qDebug() << x << "," << y;
         tileStore_->GetTile(map_[width_ * y + x]).Render(scene_, TILE_SIZE * x, TILE_SIZE * y, TILE_SIZE);
      }
   }

   if (showGrid_ == true)
   {
      for (uint32_t y = 0; y <= height_; ++y)
      {
         QGraphicsLineItem* line = scene_.addLine(0.0, TILE_SIZE * y, TILE_SIZE * width_, TILE_SIZE * y);
         QPen pen = line->pen();
         pen.setWidthF(0.025);
         line->setPen(pen);

         for (uint32_t x = 0; x <= width_; ++x)
         {
            QGraphicsLineItem* line = scene_.addLine(TILE_SIZE * x, 0.0, TILE_SIZE * x, TILE_SIZE * height_);
            QPen pen = line->pen();
            pen.setWidthF(0.025);
            line->setPen(pen);
         }
      }
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

   if (event->button() == Qt::LeftButton)
   {
       map_[width_ * y + x] = selectedView_->GetSelected();
   }
   else
   {
      selectedView_->SetSelected(map_[width_ * y + x]);
      map_[width_ * y + x] = 0;
   }
   
//qDebug() << x << "," << y << " <- " << map_[width_ * y + x];

   Render();

   emit Changed();

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
         exit(EXIT_FAILURE);
      }

      uint32_t width;
      uint32_t height;

      stream >> width >> height;
//qDebug() << width << height;

      SetSize(width, height);

      for (uint32_t y = 0; y < height_; ++y)
      {
         for (uint32_t x = 0; x < width_; ++x)
         {
            uint32_t tileId;
            stream >> tileId;
            map_[width_ * y + x] = indexMap[tileId];
         }
      }

      stream >> readStr;
//qDebug() << readStr;
      if (readStr != "Spawns")
      {
         exit(EXIT_FAILURE);
      }

      spawns_.clear();

      uint32_t numSpawns = 0;
      stream >> numSpawns;

      for (uint32_t i = 0; i < numSpawns; ++i)
      {
         uint32_t index, x, y;
         int32_t dx, dy;
         stream >> index >> x >> y >> dx >> dy;
         spawns_.push_back({x, y, dx, dy});
      }

      uint32_t i = 0;
      for (auto it: spawns_)
      {
qDebug() << i++ << "(" << it.x_ << it.y_ << ") (" << it.dx_ << it.dy_ <<  ")";
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

         emit Changed();
      }

      return true;
   }

   return false;
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
      uint32_t tileId = 0;

      for (uint32_t y = 0; y < height_; ++y)
      {
         tileId = map_[width_ * y + 0] - 1;
         if ((tileId != 0) &&
             ((tileId & 0x8) == 0))
         {
//qDebug() << "Left" << tileId << 0 << y;
            spawns_.push_back({0, y, -1, 0});
         }

         tileId = map_[width_ * y + width_ - 1] - 1;
         if ((tileId != 0) &&
             ((tileId & 0x2) == 0))
         {
//qDebug() << "Right" << tileId << width_ - 1 << y;
            spawns_.push_back({width_ - 1, y, 1, 0});
         }
      }

      for (uint32_t x = 0; x < width_; ++x)
      {
         tileId = map_[width_ * (0) + x] - 1;
         if ((tileId != 0) &&
             ((tileId & 0x1) == 0))
         {
//qDebug() << "Up" << tileId << x << 0;
            spawns_.push_back({x, 0, 0, -1});
         }

         tileId = map_[width_ * (height_ - 1) + x] - 1;
         if ((tileId != 0) &&
             ((tileId & 0x4) == 0))
         {
//qDebug() << "Down" << tileId << x << height_ - 1;
            spawns_.push_back({x, height_ - 1, 0, 1});
         }
      }

      stream << spawns_.size() << "\r\n";

      uint32_t i = 0;
      for (auto it: spawns_)
      {
         stream << i++ << it.x_ << it.y_ << it.dx_ << it.dy_ << "\r\n";
      }

      file.close();

      return true;
   }

   return false;
}
