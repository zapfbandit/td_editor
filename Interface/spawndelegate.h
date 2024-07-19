#pragma once

#include <QStyledItemDelegate>

#include "spawnmgr.h"



class SpawnDelegate : public QStyledItemDelegate
{
   Q_OBJECT

public:

   SpawnDelegate();

public:

   void SetSpawns(std::vector<SpawnMgr::SpawnInfo>& spawns);
   void SetEnemies(QStringList& enemiesStringList);

protected:

   void paint(QPainter* painter,
              const QStyleOptionViewItem& option,
              const QModelIndex& index) const override;

   QWidget* createEditor(QWidget *parent,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;

private:
   QStringList spawnsStringList_;
   QStringList enemiesStringList_;
};
