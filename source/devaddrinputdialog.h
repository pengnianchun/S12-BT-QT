#ifndef DEVADDRINPUTDIALOG_H
#define DEVADDRINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class DevAddrInputDialog;
}

class DevAddrInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DevAddrInputDialog(QWidget *parent = 0);
    ~DevAddrInputDialog();

    int get_start_address();

private slots:
    void on_spinBoxStartAddr_valueChanged(int arg1);

    void on_pushButtonConfirm_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DevAddrInputDialog *ui;

public:
    int StartAddr;
};

#endif // DEVADDRINPUTDIALOG_H
