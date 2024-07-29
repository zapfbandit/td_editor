#include "undoabletreewidgetitem.h"


UndoableTreeWidgetItem::UndoableTreeWidgetItem(QStringList& vals):
   QTreeWidgetItem(vals)
{}


void UndoableTreeWidgetItem::setData(int column, int role, const QVariant &value)
{
   oldValue_ = data(column, role);

   qDebug() << oldValue_ << value;

   QTreeWidgetItem::setData(column, role, value);
}


QVariant UndoableTreeWidgetItem::OldValue()
{
   return oldValue_;
}
