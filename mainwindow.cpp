#include <QProgressDialog>
#include <QFile>

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
    //1. 发送扫描节点命令 ， 显示返回内容

    int ret = -1;
    QString firmware, version;

    ret = helper->nodeCheck(CanHelper::FY169, firmware, version, 50);
    if (ret == -1) {
        qWarning() << "设备扫描失败1";
        return;
    }
    qDebug() << "扫描的设备：" << firmware << ", 版本：" << version; //这个显示在界面上
    // 如果仪表在app层，则发送excuteApp命令， 目的是将仪表转到bootloader层 ---->  上位机发送发送扫描节点命令， 仪表停留在bootloader
    if (firmware == "App" ) {
        ret = helper->excuteApp(CanHelper::FY169, 0);
        if (ret < 0) {
            qWarning() << "设备跳转失败";
            return;
        }

        qDebug() << "跳转到BootLoader层成功"; // 这个要显示在界面上面

        // 重新扫描设备
        QProgressDialog scanNodeProcess(QStringLiteral("重新扫描节点..."),QStringLiteral("取消"),0,100,this);
        scanNodeProcess.setWindowTitle(QStringLiteral("扫描节点"));
        scanNodeProcess.setModal(true);
        scanNodeProcess.show();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        bool bFind = false;
        int i = 0;
        do{

            ret = helper->nodeCheck(CanHelper::FY169, firmware, version, 50);

            if(ret >= 0 ){
                bFind = true;
            }
            if(i>99) {
                i = 0;
            }
            scanNodeProcess.setValue(i);
            i++;
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            if(scanNodeProcess.wasCanceled()){
                bFind = true;
                return;
            }
        } while(!bFind);

        qDebug() << "扫描的设备：" << firmware << ", 版本：" << version; //这个显示在界面上
    }

    // 2. 擦除 app
    qDebug() << "before erase flash.";
    ret = helper->eraseFlash(CanHelper::FY169, 0, 0, 10000);
    if (ret < 0) {
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("擦除Flash数据失败！"));
        return;
    }
    // 3. 读取镜像文件， 发送数据
    QByteArray array;
    array.resize(1026);
    for (int i = 0; i < 1026; i ++) {
        array[i] = 0x00;
    }

    QFile firmwareFile("F:\\new1.s19");
    if (!firmwareFile.open(QFile::ReadOnly)) {
        qWarning() << "打开镜像文件失败";
        return;
    }
    quint8 FirmwareData[1026]={0};
    int read_data_num;
    QProgressDialog writeDataProcess(QStringLiteral("正在更新固件..."),QStringLiteral("取消"),0,firmwareFile.size(),this);
    writeDataProcess.setWindowTitle(QStringLiteral("更新固件"));
    writeDataProcess.setModal(true);
    writeDataProcess.show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    int i=0;
    for(i=0;i<firmwareFile.size();i+=1024){
       read_data_num = firmwareFile.read((char*)FirmwareData,1024);
       uint16_t crc_data = helper->crc16Ccitt(FirmwareData,read_data_num);
       FirmwareData[read_data_num] = crc_data>>8;
       FirmwareData[read_data_num+1] = crc_data;
       read_data_num += 2;
       QByteArray array((const char*)FirmwareData, read_data_num);
       //qDebug() << "array size:" << array.size() << ", read num:" << read_data_num;
       ret = helper->updateFirmware(CanHelper::FY169, 0, array, 5000);
       if (ret < 0) {
           QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("写Flash数据失败！"));
           return;
       }
       writeDataProcess.setValue(i);
       QCoreApplication::processEvents(QEventLoop::AllEvents);
       if(writeDataProcess.wasCanceled()){
           return;
       }
    }

    writeDataProcess.setValue(firmwareFile.size());
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    if(writeDataProcess.wasCanceled()){

        return;
    }
    return;

    qDebug() << "ret: " << ret;

}

void MainWindow::on_pushButton_sync_3_clicked()
{
    int ret = -1;
    ret = helper->excuteApp(CanHelper::FY169, 0);
    if (ret < 0) {
        qWarning() << "设备跳转失败";
        return;
    }

    qDebug() << "跳转成功"; // 这个要显示在界面上面
}
