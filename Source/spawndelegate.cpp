#include "spawndelegate.h"

#include <QComboBox>
#include <QSpinBox>


SpawnDelegate::SpawnDelegate()
{}


void SpawnDelegate::SetSpawns(std::vector<MapView::Spawn>& spawns)
{
   spawnsStringList_.clear();
   for (MapView::Spawn spawn: spawns)
   {
      QString name = QString("%0 (%1,%2)").arg(spawn.index_).arg(spawn.x_).arg(spawn.y_);
      spawnsStringList_.push_front(name);
   }
}


void SpawnDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   return QStyledItemDelegate::paint(painter, option, index);
}



QWidget* SpawnDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
   qDebug() << "SpawnDelegate::createEditor";


   if (index.column() == 1)
   {
      QSpinBox* spin = new QSpinBox(parent);
      spin->setMinimum(0);
      spin->setMaximum(100);
      spin->setSuffix("%");
      return spin;
   }

   if (index.column() == 2)
   {
      QComboBox* combo = new QComboBox(parent);
      combo->addItems(spawnsStringList_);
      return combo;
   }

   if (index.column() == 3)
   {
      QSpinBox* spin = new QSpinBox(parent);
      spin->setMinimum(1);
      spin->setMaximum(8);
      return spin;
   }

   if (index.column() == 4)
   {
      QComboBox* combo = new QComboBox(parent);
      combo->addItems(QStringList() << "Type");
      return combo;
   }

   return QStyledItemDelegate::createEditor(parent, option, index);
}
