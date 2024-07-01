#pragma once

#include <QMainWindow>

#include "tilestore.h"
#include "pixmapstore.h"
#include "spritemgr.h"

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

   void NewMap();
   void OpenMap();
   void SaveMap();
   void SaveMapAs();

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

   TileStore tileStore_;
   PixmapStore pixmapStore_;

   SpriteMgr spriteMgr_;

   QString mapPath_;
   bool dirty_;

   Game game_;
};
