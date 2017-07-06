#include "devaddrinputdialog.h"
#include "ui_devaddrinputdialog.h"

DevAddrInputDialog::DevAddrInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevAddrInputDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->comboBox->setCurrentIndex(0); // Set default value
    this->setFixedSize(399, 164);
#ifdef LANGUE_EN
    //this->setWindowTitle("Scan Node");
    //ui->label->setText("Please input addr(Hex)：");
    //ui->pushButtonConfirm->setText("OK");
    //ui->pushButtonCancel->setText("Cancel");
#else
    this->setWindowTitle("选择设备");
    //ui->label->setText("      请选择设备：");
    //ui->pushButtonConfirm->setText("确定");
    //ui->pushButtonCancel->setText("取消");
#endif
}

DevAddrInputDialog::~DevAddrInputDialog()
{
    delete ui;
}

void DevAddrInputDialog::on_pushButtonConfirm_clicked()
{
    this->accept();
}

void DevAddrInputDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}

int DevAddrInputDialog::get_device_addr() {
    int addr = 0;
    if( 0 == ui->comboBox->currentIndex()) {
        addr = 0x12;
    } else if (1 == ui->comboBox->currentIndex()) {
        addr = 0x13;
    }
    return addr;
}
