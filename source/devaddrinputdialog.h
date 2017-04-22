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

private slots:
    void on_spinBoxStartAddr_valueChanged(int arg1);

    void on_pushButtonConfirm_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DevAddrInputDialog *ui;

public:
    int StartAddr,EndAddr;
};

#endif // DEVADDRINPUTDIALOG_H
