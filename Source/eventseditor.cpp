#include "eventseditor.h"


EventsEditor::EventsEditor(Ui::MainWindow* ui):
   ui_(ui)
{}


void EventsEditor::ApplyNumStages()
{
   QTreeWidgetItem* item;

   for (int row = 1; row <= ui_->numStagesSpinBox_->value(); ++row)
   {
      if (ui_->eventTreeWidget_->findItems(QString::number(row), Qt::MatchExactly, 0).size() == 0)
      {
         QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << QString::number(row));

         ui_->eventTreeWidget_->addTopLevelItem(item);
      }
   }
}
