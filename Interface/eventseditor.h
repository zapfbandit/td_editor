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

signals:

   void Changed();
   void Created();
   void Destroyed();

public slots:

   void ApplyNumStages();

   void Add();
   void Remove();

   void ItemSelectionChanged();

   void ItemChanged(QTreeWidgetItem * item, int column);

private:

   Ui::MainWindow* ui_;

   QTreeWidgetItem* selItem_;
   QTreeWidgetItem* topSel_;
};
