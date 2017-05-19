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

    int get_device_addr();

private slots:
    void on_pushButtonConfirm_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DevAddrInputDialog *ui;

};

#endif // DEVADDRINPUTDIALOG_H
