#pragma once

#include "ui_mainwindow.h"


class EventsEditor: public QObject
{
   Q_OBJECT

public:

   EventsEditor(Ui::MainWindow* ui);

public slots:

   void ApplyNumStages();

private:

   Ui::MainWindow* ui_;
};
