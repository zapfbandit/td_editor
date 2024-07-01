#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
   Q_OBJECT

public:
   explicit NewDialog(QWidget*       parent,
                      const uint32_t width,
                      const uint32_t height);

   ~NewDialog();

public:

   uint32_t Width() const;
   uint32_t Height() const;

private:

   Ui::NewDialog* ui;

};

#endif // NEWDIALOG_H
