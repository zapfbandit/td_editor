#include "eventseditor.h"


EventsEditor::EventsEditor(Ui::MainWindow* ui):
   ui_(ui)
{}


void EventsEditor::ApplyNumStages()
{
   QTreeWidgetItem* item;

   int numStages = ui_->numStagesSpinBox_->value();

   // Out with the old
   bool changed;
   do
   {
      changed = false;
      int numItems = ui_->eventTreeWidget_->topLevelItemCount();
      for (int row = 0; (changed == false) && (row < numItems); ++row)
      {
         QTreeWidgetItem* item = ui_->eventTreeWidget_->topLevelItem(row);
         if (item->data(0, Qt::DisplayRole).toInt() > numStages)
         {
            qDebug() << item->data(0, Qt::DisplayRole).toInt();
            delete ui_->eventTreeWidget_->takeTopLevelItem(row);
            changed = true;
         }
      }
   }
   while (changed == true);

   // And in with the new
   for (int row = 1; row <= numStages; ++row)
   {
      if (ui_->eventTreeWidget_->findItems(QString::number(row), Qt::MatchExactly, 0).size() == 0)
      {
         QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << QString::number(row));

         ui_->eventTreeWidget_->addTopLevelItem(item);
      }
   }
}
