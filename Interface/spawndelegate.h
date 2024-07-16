#pragma once

#include <QStyledItemDelegate>

#include "mapview.h"



class SpawnDelegate : public QStyledItemDelegate
{
   Q_OBJECT

public:

   SpawnDelegate();

public:

   void SetSpawns(std::vector<MapView::Spawn>& spawns);

protected:

   void paint(QPainter* painter,
              const QStyleOptionViewItem& option,
              const QModelIndex& index) const override;

   QWidget* createEditor(QWidget *parent,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;

private:
   QStringList spawnsStringList_;
};
