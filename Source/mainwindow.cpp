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
   dirty_(false),
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

   connect(ui->mapView_, &MapView::Changed,
           this,         &MainWindow::MakeDirty);
   
   connect(ui->actionShowGrid_, &QAction::triggered,
           this,                &MainWindow::ShowGrid);

   connect(ui->actionNew_, &QAction::triggered,
           this,           &MainWindow::NewMap);

   connect(ui->actionOpen_, &QAction::triggered,
           this,            &MainWindow::OpenMap);

   connect(ui->actionSave_, &QAction::triggered,
           this,            &MainWindow::SaveMap);

   connect(ui->actionSaveAs_, &QAction::triggered,
           this,              &MainWindow::SaveMapAs);

   const bool showGrid = settings_.ShowGrid();
   ui->actionShowGrid_->setChecked(showGrid);
   ui->mapView_->ShowGrid(showGrid);

   restoreState(settings_.WindowState());
   restoreGeometry(settings_.WindowGeometry());

   spriteMgr_.Init(ui->mapView_->scene(), &pixmapStore_);

   QTimer::singleShot(0, this, &MainWindow::OpenLastMap);

   QTimer::singleShot(0, this, &MainWindow::Tick);
}


MainWindow::~MainWindow()
{
   settings_.WindowState(saveState());
   settings_.WindowGeometry(saveGeometry());

   delete ui;
}


void MainWindow::OpenLastMap()
{
   DoOpenMap(settings_.MapPath());

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

               qDebug() << px << py;
            }
            while (px*px + py*py > 1);

            qDebug() << "Ok";

            spriteMgr_.Add(x + 0.25*px, y + 0.25*py, dx, dy, 48.0/64.0, "Enemies", "Zombie", 6, 2.5, 0.05);
         }
      }
   }
   else
   {
      spriteMgr_.Add(0.5, 0.5, 1, 0, 1, "Enemies", "Zombie", 6, 2, 0.1);

      spriteMgr_.Add(0.5, 0.5, 0, 1, 2, "Enemies", "Zombie - Big", 6, 2, 0.1);
   }
}


void MainWindow::Tick()
{
   game_.Tick();

   QTimer::singleShot(100, this, &MainWindow::Tick);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (dirty_ == true)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this,
            tr("TD Editor"),
            tr("Are you sure you want to discard changes to this map ?"),
            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
            QMessageBox::Yes);

        if (resBtn != QMessageBox::Yes)
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


void MainWindow::SetMapPath(const QString& mapPath)
{
   mapPath_ = mapPath;

   RedrawTitle();
}


void MainWindow::MakeDirty()
{
    dirty_ = true;

    RedrawTitle();
}


void MainWindow::MakeClean()
{
    dirty_ = false;

    RedrawTitle();
}


void MainWindow::RedrawTitle()
{
    QString windowTitle = "TD Editor";

    if (mapPath_.isEmpty() == false)
    {
        windowTitle += " - " + QFileInfo(mapPath_).fileName();
    }

    if (dirty_ == true)
    {
        windowTitle += " *";
    }

    setWindowTitle(windowTitle);

    ui->actionSave_->setDisabled(false);
    ui->actionSaveAs_->setDisabled(false);
}
