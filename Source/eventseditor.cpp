#include "eventseditor.h"

#include <QTreeWidget>

#include "spawndelegate.h"


EventsEditor::EventsEditor(Ui::MainWindow* ui):
   ui_(ui)
{}


void EventsEditor::Setup()
{
   SpawnDelegate* delegate = new SpawnDelegate;
   ui_->eventTreeWidget_->setItemDelegate(delegate);
   ui_->eventTreeWidget_->setEditTriggers(QAbstractItemView::AllEditTriggers);

   ui_->addPushButton_->setEnabled(false);
   ui_->removePushButton_->setEnabled(false);
}


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


void EventsEditor::Add()
{
   qDebug() << "EventsEditor::Add";

   QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList() << "" << "50 %" << "0 (0, 0)" << "3" << "Zombie");
   newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);

   int pos = 0;

   if (selItem_ != topSel_)
   {
      pos = topSel_->indexOfChild(selItem_) + 1;
   }

   qDebug() << pos;

   topSel_->insertChild(pos, newItem);
   topSel_->setExpanded(true);
   ui_->eventTreeWidget_->setCurrentItem(newItem);
}


void EventsEditor::Remove()
{
   qDebug() << "EventsEditor::Remove" << topSel_ << selItem_;

   QTreeWidgetItem* selItem = selItem_;
   topSel_->removeChild(selItem);
   delete selItem;
}


void EventsEditor::ItemSelectionChanged()
{
   qDebug() << "EventsEditor::ItemSelectionChanged";

   selItem_ = nullptr;
   QList<QTreeWidgetItem*> selItems = ui_->eventTreeWidget_->selectedItems();
   if (selItems.count() > 0)
   {
      selItem_ = selItems.first();

      topSel_ = selItem_;
      while (topSel_->parent() != nullptr)
      {
         topSel_ = topSel_->parent();
      }

      ui_->addPushButton_->setEnabled(true);

      ui_->removePushButton_->setEnabled(selItem_ == topSel_ ? false : true);
   }
   else
   {
      ui_->addPushButton_->setEnabled(false);
      ui_->removePushButton_->setEnabled(false);
   }
}
