#include "spawndelegate.h"

#include <QComboBox>


SpawnDelegate::SpawnDelegate()
{}


void SpawnDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   return QStyledItemDelegate::paint(painter, option, index);
}



QWidget* SpawnDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
   qDebug() << "SpawnDelegate::createEditor";


   if (index.column() == 2)
   {
      QComboBox* combo = new QComboBox(parent);
      combo->addItems(QStringList() << "Spawn");
      return combo;
   }

   if (index.column() == 4)
   {
      QComboBox* combo = new QComboBox(parent);
      combo->addItems(QStringList() << "Type");
      return combo;
   }

   return QStyledItemDelegate::createEditor(parent, option, index);
}
