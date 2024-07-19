#pragma once

#include <QStyledItemDelegate>


class PercentDelegate : public QStyledItemDelegate
{
   Q_OBJECT

public:

   PercentDelegate();

public:

   QString displayText(const QVariant &value, const QLocale &locale) const;

protected:

   QWidget* createEditor(QWidget *parent,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;
};
