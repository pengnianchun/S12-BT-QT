#include "devaddrinputdialog.h"
#include "ui_devaddrinputdialog.h"

DevAddrInputDialog::DevAddrInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevAddrInputDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->spinBoxStartAddr->setValue(18); // Set default value

#ifdef LANGUE_EN
    this->setWindowTitle("Scan Node");
    ui->label->setText("Please input addr(Hex)：");
    ui->pushButtonConfirm->setText("OK");
    ui->pushButtonCancel->setText("Cancel");
#else
    this->setWindowTitle("结点扫描");
    ui->label->setText("请输入结点地址(Hex)：");
    ui->pushButtonConfirm->setText("确定");
    ui->pushButtonCancel->setText("取消");
#endif
}

DevAddrInputDialog::~DevAddrInputDialog()
{
    delete ui;
}

void DevAddrInputDialog::on_spinBoxStartAddr_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
}

void DevAddrInputDialog::on_pushButtonConfirm_clicked()
{
    StartAddr = ui->spinBoxStartAddr->value();
    this->accept();
}

void DevAddrInputDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}
