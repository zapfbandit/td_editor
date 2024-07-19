#include "percentdelegate.h"

#include <QSpinBox>

PercentDelegate::PercentDelegate()
{}


QString PercentDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
   return value.toString() + " %";
}


QWidget* PercentDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
   if (index.column() == 1)
   {
      QSpinBox* spin = new QSpinBox(parent);
      spin->setMinimum(0);
      spin->setMaximum(100);
      spin->setSuffix(" %");
      return spin;
   }

   return nullptr;
}
