#pragma once

#include <QMainWindow>

#include "tilestore.h"
#include "pixmapstore.h"
#include "spritemgr.h"

#include "eventseditor.h"

#include "settings.h"
#include "game.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

public slots:

   void OpenLastMap();
   void Tick();

   void ShowGrid();
   void MakeSpawns();

   void NewMap();
   void OpenMap();
   void SaveMap();
   void SaveMapAs();

   void Undo();
   void Redo();

   void MakeChange(uint32_t x, uint32_t y, uint32_t oldTile, uint32_t newTile);
   void MakeDirty();
   void MakeClean();

private:
   void SetMapPath(const QString& mapPath);
   void DoOpenMap(const QString& mapPath);
   void RedrawTitle();

protected:
   virtual void closeEvent(QCloseEvent *event) override;

private:

   Settings settings_;

   Ui::MainWindow *ui;

   EventsEditor eventsEditor_;

   TileStore tileStore_;
   PixmapStore pixmapStore_;

   SpriteMgr spriteMgr_;

   QString mapPath_;

   Game game_;

   struct UndoEntry
   {
      uint32_t x_;
      uint32_t y_;
      uint32_t oldTile_;
      uint32_t newTile_;
   };

   std::vector<UndoEntry> undoStack_;
   std::vector<UndoEntry> redoStack_;

   uint32_t saveUndoCount_;

};
