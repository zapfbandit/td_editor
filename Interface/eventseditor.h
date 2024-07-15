#pragma once

#include "ui_mainwindow.h"


class EventsEditor: public QObject
{
   Q_OBJECT

public:

   EventsEditor(Ui::MainWindow* ui);

public slots:

   void ApplyNumStages();
   void Add();
   void Remove();

private:

   Ui::MainWindow* ui_;
};
