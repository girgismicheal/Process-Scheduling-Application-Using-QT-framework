#ifndef SHOW_DIALOG_H
#define SHOW_DIALOG_H

#include <QDialog>

namespace Ui {
class SHOW_Dialog;
}

class SHOW_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SHOW_Dialog(float wait,QWidget *parent = nullptr);
    ~SHOW_Dialog();
    void set(float wait);

private slots:
    void on_pushButton_clicked();


private:
    Ui::SHOW_Dialog *ui;
};

#endif // SHOW_DIALOG_H
