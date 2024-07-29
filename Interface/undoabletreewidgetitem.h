#pragma once

#include <QTreeWidgetItem>


class UndoableTreeWidgetItem : public QTreeWidgetItem
{
public:
   UndoableTreeWidgetItem(QStringList& vals);

public:
   void setData(int column, int role, const QVariant &value) final;

   QVariant OldValue();

private:
   QVariant oldValue_;
};
