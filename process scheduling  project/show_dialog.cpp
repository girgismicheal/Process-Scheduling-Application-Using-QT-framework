#include "show_dialog.h"
#include "ui_show_dialog.h"
SHOW_Dialog::SHOW_Dialog(float wait,QWidget *parent) :QDialog(parent),ui(new Ui::SHOW_Dialog)
{
    ui->setupUi(this);
    QString b;
    b.setNum(wait);
    ui->label_2->setText(b);
}

SHOW_Dialog::~SHOW_Dialog()
{
    delete ui;
}
void SHOW_Dialog::on_pushButton_clicked()
{
    SHOW_Dialog:: close();
}
