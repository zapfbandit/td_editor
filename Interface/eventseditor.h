#pragma once

#include "ui_mainwindow.h"

#include <QTreeWidgetItem>


class EventsEditor: public QObject
{
   Q_OBJECT

public:

   EventsEditor(Ui::MainWindow* ui);

public:

   void Setup(SpawnDelegate* spawnDelegate);

public slots:

   void ApplyNumStages();

   void Add();
   void Remove();

   void ItemSelectionChanged();

private:

   Ui::MainWindow* ui_;

   QTreeWidgetItem* selItem_;
   QTreeWidgetItem* topSel_;
};
