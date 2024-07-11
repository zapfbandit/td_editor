#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

#include "newdialog.h"


MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   tileStore_(settings_.TilesPath()),
   pixmapStore_(settings_.SpritesPath()),
   spriteMgr_(1024),
   saveUndoCount_(0),
   game_(spriteMgr_)
{
   ui->setupUi(this);

   ui->mapView_->SetSettings(&settings_);

   ui->tilesView_->SetTileStore(&tileStore_);
   ui->selectedView_->SetTileStore(&tileStore_);
   ui->mapView_->SetTileStore(&tileStore_);

   ui->tilesView_->SetSelectedView(ui->selectedView_);
   ui->mapView_->SetSelectedView(ui->selectedView_);

   ui->actionSave_->setDisabled(true);
   ui->actionSaveAs_->setDisabled(true);

   connect(ui->mapView_, &MapView::Touched,
           this,         &MainWindow::MakeDirty);

   connect(ui->mapView_, &MapView::Changed,
           this,         &MainWindow::MakeChange);

   connect(ui->actionNew_, &QAction::triggered,
           this,           &MainWindow::NewMap);

   connect(ui->actionOpen_, &QAction::triggered,
           this,            &MainWindow::OpenMap);

   connect(ui->actionSave_, &QAction::triggered,
           this,            &MainWindow::SaveMap);

   connect(ui->actionSaveAs_, &QAction::triggered,
           this,              &MainWindow::SaveMapAs);

   connect(ui->actionUndo_, &QAction::triggered,
           this,            &MainWindow::Undo);

   connect(ui->actionRedo_, &QAction::triggered,
           this,            &MainWindow::Redo);

   connect(ui->actionShowGrid_, &QAction::triggered,
           this,                &MainWindow::ShowGrid);

   const bool showGrid = settings_.ShowGrid();
   ui->actionShowGrid_->setChecked(showGrid);
   ui->mapView_->ShowGrid(showGrid);

   restoreState(settings_.WindowState());
   restoreGeometry(settings_.WindowGeometry());

   spriteMgr_.Init(ui->mapView_->scene(), &pixmapStore_, ui->mapView_);

   QTimer::singleShot(0, this, &MainWindow::OpenLastMap);

   QTimer *timer = new QTimer(this);
   timer->start(100);
   connect(timer, &QTimer::timeout,
           this, &MainWindow::Tick);

   //QTimer::singleShot(0, this, &MainWindow::Tick);
}


MainWindow::~MainWindow()
{
   settings_.WindowState(saveState());
   settings_.WindowGeometry(saveGeometry());

   delete ui;
}


void MainWindow::MakeSpawns()
{
   uint32_t numSpawns = ui->mapView_->NumSpawns();

   if (numSpawns > 0)
   {
      for (uint32_t i = 0; i < numSpawns; ++i)
      {
         MapView::Spawn spawn = ui->mapView_->GetSpawn(i);

         double x = spawn.x_ + spawn.dx_ + 0.5;
         double dx = -spawn.dx_;
         double y = spawn.y_ + spawn.dy_ + 0.5;
         double dy = -spawn.dy_;

         qDebug() << x << y << dx << dy;

         for (uint32_t i = 0; i < 3; ++i)
         {
            double px = 0;
            double py = 0;
            do
            {
               px = (rand() - RAND_MAX/2.0) / (RAND_MAX/2.0);
               py = (rand() - RAND_MAX/2.0) / (RAND_MAX/2.0);
            }
            while (px*px + py*py > 1);

            spriteMgr_.Add(x + 0.2*px, y + 0.2*py, dx, dy, 0.5, "Enemies", "Bat", 6, 10, 0.2);
         }
      }
   }
   else
   {
      //spriteMgr_.Add(0.5, 0.5, 1, 0, 1, "Enemies", "Zombie", 6, 2, 0.1);

      //spriteMgr_.Add(0.5, 0.5, 0, 1, 2, "Enemies", "Zombie - Big", 6, 2, 0.1);
   }
}


void MainWindow::OpenLastMap()
{
   DoOpenMap(settings_.MapPath());
}


void MainWindow::Tick()
{
   game_.Tick();

   ui->statusbar->showMessage(QString("Sprites = %0").arg(spriteMgr_.NumSprites()));

   //QTimer::singleShot(100, this, &MainWindow::Tick);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (undoStack_.size() != saveUndoCount_)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this,
            tr("TD Editor"),
            tr("Do you want to save changes to the map before quitting?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Yes);

        if (resBtn == QMessageBox::Yes)
        {
           SaveMap();
        }
        else if (resBtn == QMessageBox::Cancel)
        {
           event->ignore();
           return;
        }
    }

    event->accept();
}


void MainWindow::ShowGrid()
{
   bool showGrid = ui->actionShowGrid_->isChecked();
   settings_.ShowGrid(showGrid);
   ui->mapView_->ShowGrid(showGrid);
}


void MainWindow::NewMap()
{
   NewDialog dialog(this, settings_.MapWidth(), settings_.MapHeight());
   int choice = dialog.exec();
   if (choice == QDialog::Accepted)
   {
      ui->mapView_->SetSize(dialog.Width(), dialog.Height());

      settings_.MapWidth(dialog.Width());
      settings_.MapHeight(dialog.Height());

      SetMapPath("");
      MakeDirty();
   }
}


void MainWindow::OpenMap()
{
   QString mapPath =
        QFileDialog::getOpenFileName(this,
                                     tr("Open Map"),
                                     settings_.MapPath(),
                                     tr("Map (*.map)"));

   DoOpenMap(mapPath);
}


void MainWindow::DoOpenMap(const QString& mapPath)
{
   if (mapPath.isEmpty() == false)
   {
      SetMapPath(mapPath);
      MakeClean();

      if (ui->mapView_->OpenFile(mapPath) == true)
      {
         settings_.MapPath(mapPath);
      }
      else
      {
         qDebug() << "An error occured while openning a map.";
         SetMapPath("");
         MakeClean();
      }
   }
}


void MainWindow::SaveMap()
{
   if (mapPath_.isEmpty() == true)
   {
      SaveMapAs();
   }
   else
   {
      if (ui->mapView_->SaveToFile(mapPath_) == true)
      {
         SetMapPath(mapPath_);
         MakeClean();
      }
      else
      {
         qDebug() << "An error occured while saving a map.";
      }
   }

   OpenLastMap();
   MakeSpawns();
}


void MainWindow::SaveMapAs()
{
   static QString mapPath;

   mapPath = QFileDialog::getSaveFileName(this,
                                          tr("Save Map"),
                                          settings_.MapPath(),
                                          tr("Map (*.map)"));

   if (mapPath.isEmpty() == false)
   {
      if (ui->mapView_->SaveToFile(mapPath) == true)
      {
         settings_.MapPath(mapPath);
         SetMapPath(mapPath);
         MakeClean();
      }
      else
      {
         qDebug() << "An error occured while saving a map.";
      }
   }
}


void MainWindow::Undo()
{
   UndoEntry entry = undoStack_.back();
   undoStack_.pop_back();

   ui->mapView_->DoChange(entry.x_, entry.y_, entry.oldTile_);

   redoStack_.push_back(entry);

   RedrawTitle();
}


void MainWindow::Redo()
{
   UndoEntry entry = redoStack_.back();
   redoStack_.pop_back();

   ui->mapView_->DoChange(entry.x_, entry.y_, entry.newTile_);

   undoStack_.push_back(entry);

   RedrawTitle();
}


void MainWindow::SetMapPath(const QString& mapPath)
{
   mapPath_ = mapPath;

   RedrawTitle();
}


void MainWindow::MakeChange(uint32_t x, uint32_t y, uint32_t oldTile, uint32_t newTile)
{
   undoStack_.push_back({x, y, oldTile, newTile});

   RedrawTitle();
}


void MainWindow::MakeDirty()
{
    saveUndoCount_ = UINT32_MAX;

    RedrawTitle();
}


void MainWindow::MakeClean()
{
    saveUndoCount_ = undoStack_.size();

    RedrawTitle();
}


void MainWindow::RedrawTitle()
{
    QString windowTitle = "TD Editor";

    if (mapPath_.isEmpty() == false)
    {
        windowTitle += " - " + QFileInfo(mapPath_).fileName();
    }

    if (undoStack_.size() != saveUndoCount_)
    {
        windowTitle += " *";
    }

    setWindowTitle(windowTitle);

    ui->actionSave_->setDisabled(false);
    ui->actionSaveAs_->setDisabled(false);

    ui->actionUndo_->setDisabled(undoStack_.size() == 0);
    ui->actionRedo_->setDisabled(redoStack_.size() == 0);
}
