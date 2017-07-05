#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    helper(new CanHelper)
{

    ui->setupUi(this);
    this->setFixedSize(800, 480);
    this->setWindowTitle(tr("时间同步工具 FY-299 Ver1.0.0"));

    onTimerOut();
    timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    m_DevType = (m_nDevType == 1) ?  VCI_USBCAN2 :  VCI_USBCAN1;

    ui->pushButton_open->setEnabled(true);
    ui->pushButton_sync->setEnabled(false);
    ui->pushButton_close->setEnabled(false);

    QFont font1;
    font1.setPointSize(22);
    ui->labelTitle->setFont(font1);

    QFont font2;
    font2.setPointSize(32);
    ui->labelCurrentTime->setFont(font2);

    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::red);
    ui->labelTips->setPalette(palette);

}

MainWindow::~MainWindow()
{
    if(NULL != timer) {
        delete timer;
        timer = NULL;
    }
    delete ui;
    helper->close();
    delete helper;
}



void MainWindow::on_pushButton_open_clicked()
{

    if ( -1 == helper->open(m_nCanIndex) ) {
        ui->labelTips->setText(tr("打开设备失败"));
        return;
    }

    ui->pushButton_close->setEnabled(true);
    ui->pushButton_sync->setEnabled(true);
    ui->pushButton_open->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->labelTips->setText(tr("打开设备成功"));
    displayFlag = true;
    stepCount = 0;
  //  recvMsg->start();
}

void MainWindow::on_pushButton_sync_clicked()
{

    ////////////////////////////////////////////////////////////////
    //测试设备选择
    QString appVersion;
    QString firmware;
    qDebug() << "push button .....";
    int ret = helper->nodeCheck(CanHelper::FY169, firmware, appVersion, 50);
    if (ret < 0) {
        qDebug() << "没有设备";
    }
    qDebug() << "version:" << appVersion;
    ///////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////
    //测试设备选择
//    qDebug() << "eraseFlash test .....";
//    helper->eraseFlash(CanHelper::FY169, 100, 100, 6000);

    ///////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////
    //测试设备选择

    ///////////////////////////////////////////////////////////////
    return;
}

void MainWindow::on_pushButton_close_clicked()
{
    if (-1 == helper->close()) {
        ui->labelTips->setText(tr("设备关闭失败"));
        return;
    }
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_sync->setEnabled(false);
    ui->comboBox->setEnabled(true);
    ui->labelTips->setText(tr("设备已关闭"));
    displayFlag = true;
    stepCount = 0;

}

void MainWindow::onTimerOut() {
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->labelCurrentTime->setText(str);
    if(true == displayFlag) {
        stepCount++;
        if(stepCount>5) {
            ui->labelTips->setText(tr(""));
            displayFlag = false;
            stepCount = 0;
        }
    }

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if((0 == index) || (1 == index)) {
        m_nCanIndex = 0;
    } else {
        m_nCanIndex = 1;
    }

}

void MainWindow::on_pushButton_sync_2_clicked()
{
    qDebug() << "update firm ware test .....";
    QByteArray array;
    array.resize(1026);
    for (int i = 0; i < 1026; i ++) {
        array[i] = 0x01;
    }
    int ret = helper->updateFirmware(CanHelper::FY169, 0, array, 5000);

    qDebug() << "ret: " << ret;

}
