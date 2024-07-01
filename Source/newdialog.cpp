#include "newdialog.h"
#include "ui_newdialog.h"

NewDialog::NewDialog(QWidget *parent,
                     const uint32_t width,
                     const uint32_t height):
   QDialog(parent),
   ui(new Ui::NewDialog)
{
   ui->setupUi(this);

   ui->widthSpinBox_->setValue(width);
   ui->heightSpinBox_->setValue(height);
}


NewDialog::~NewDialog()
{
   delete ui;
}


uint32_t NewDialog::Width() const
{
   return ui->widthSpinBox_->value();
}


uint32_t NewDialog::Height() const
{
   return ui->heightSpinBox_->value();
}
