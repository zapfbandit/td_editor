#pragma once

#include <QStyledItemDelegate>

class SpawnDelegate : public QStyledItemDelegate
{
   Q_OBJECT

public:

   SpawnDelegate();

public:

   void paint(QPainter* painter,
              const QStyleOptionViewItem& option,
              const QModelIndex& index) const override;

   QWidget* createEditor(QWidget *parent,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;
};
