#include "spawndelegate.h"

#include <QComboBox>
#include <QSpinBox>


SpawnDelegate::SpawnDelegate()
{}


void SpawnDelegate::SetSpawns(std::vector<SpawnMgr::SpawnInfo>& spawns)
{
   struct Arrow
   {
      int dx;
      int dy;
      QString str;
   };

   std::vector<Arrow> arrows = {{ 0, -1, "↑"},
                                { 1,  0, "→"},
                                { 0,  1, "↓"},
                                {-1,  0, "←"}};

   spawnsStringList_.clear();
   for (SpawnMgr::SpawnInfo spawn: spawns)
   {
      QString arrowStr = "";
      for (Arrow& arrow: arrows)
      {
         if ((spawn.dx_ == arrow.dx) &&
             (spawn.dy_ == arrow.dy))
         {
            arrowStr = arrow.str;
         }
      }
      QString name = QString("(%1,%2) %3").arg(spawn.x_).arg(spawn.y_).arg(arrowStr);
      spawnsStringList_.push_back(name);
   }
}


void SpawnDelegate::SetEnemies(QStringList& enemiesStringList)
{
   enemiesStringList_ = enemiesStringList;
}


QString SpawnDelegate::SpawnStr(const uint32_t index) const
{
   if (index < spawnsStringList_.size())
   {
      return spawnsStringList_[index];
   }

   return "";
}


uint32_t SpawnDelegate::SpawnIndex(const QString str) const
{
   for (uint32_t i = 0 ; i < spawnsStringList_.size(); ++i)
   {
      if (str == spawnsStringList_[i])
      {
         return i;
      }
   }

   return spawnsStringList_.size();
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


   if (index.column() == 0)
   {
      return nullptr;
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
      combo->addItems(enemiesStringList_);
      return combo;
   }

   return QStyledItemDelegate::createEditor(parent, option, index);
}
