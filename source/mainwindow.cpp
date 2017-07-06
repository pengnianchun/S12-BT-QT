#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800, 480);
    this->setWindowTitle(tr("CAN固件下载工具 Ver1.0.0"));
    ui->statusBar->showMessage("欢迎使用CAN固件下载工具!", 8000);
    //ui->comboBox_selectCan->setCurrentIndex(1);
    ui->label_firmwareType->setText("未知类型");
    ui->label_firmwareVersion->setText("未知版本");

    labelTag = new QLabel();
    ui->statusBar->addPermanentWidget(labelTag);
}

MainWindow::~MainWindow()
{
    helper->close();
    delete helper;
    delete ui;
}

void MainWindow::outputInformation(QString log) {
    QString temp = "";
    qDebug()<<" --- echo text = "<<log;
    temp.append(QDateTime::currentDateTime().toString("[MM/dd hh:mm:ss:zzz]   "));
    temp.append(log);
    ui->textEdit_outputLog->append(temp);
}

void MainWindow::on_openFirmwareFileAction_triggered()
{
    on_openFirmwareFilePushButton_clicked();
}

void MainWindow::on_scanNodeAction_triggered()
{
    on_pushButton_scanNode_clicked();
}

void MainWindow::on_clearDisplayAction_triggered()
{
    ui->textEdit_outputLog->clear();
}

void MainWindow::on_aboutAction_triggered()
{
    QString AboutStr;
    AboutStr = "S12 USB-CAN Bootloader 1.0.0\n";
    AboutStr.append("Shanghai Fangyan 2015-2017 Copyright\n");
    AboutStr.append("Hardware Support: S12 USB-CAN Adapter");
    QMessageBox::about(this,"About S12 USB-CAN Bootloader",AboutStr);
}

void MainWindow::on_exitAction_triggered()
{
    this->close();
}

void MainWindow::on_comboBox_selectCan_currentIndexChanged(int index)
{
    if(0 == index) {
        m_nCanIndex = 0;
    } else if(1 == index){
        m_nCanIndex = 0;
        outputInformation(tr("当前已选择CAN1通道"));
    } else {
        m_nCanIndex = 1;
        outputInformation(tr("当前已选择CAN2通道"));
    }
}

void MainWindow::on_pushButton_openCan_clicked()
{
    QString tipsInfo;
    if ( -1 == helper->open(m_nCanIndex) ) {
        tipsInfo = "CAN设备打开失败 !";
        ui->statusBar->showMessage(tipsInfo, 3000);
        outputInformation(tipsInfo);
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("CAN设备打开失败 !\t\t\n");
        msgBox.exec();
        return;
    }
    tipsInfo = "CAN设备打开成功 !";
    ui->statusBar->showMessage(tipsInfo, 3000);
    outputInformation(tipsInfo);
}

void MainWindow::on_pushButton_closeCan_clicked()
{
    QString tipsInfo;
    if (-1 == helper->close()) {
        tipsInfo = "CAN设备关闭失败 !";
        ui->statusBar->showMessage(tipsInfo, 3000);
        return;
    }
    tipsInfo = "CAN设备已关闭 !";
    ui->statusBar->showMessage(tipsInfo, 3000);
    outputInformation(tipsInfo);
}

void MainWindow::on_openFirmwareFilePushButton_clicked()
{
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,
                                          tr("Open files"),
                                          "",
                                          "S19 Files (*.s19);;Binary Files (*.bin);;Hex Files (*.hex);;All Files (*.*)");
    if(fileName.isNull()){
        return;
    }
    ui->firmwareLineEdit->setText(fileName);
}

void MainWindow::on_pushButton_scanNode_clicked()
{
    DevAddrInputDialog *pDevAddrInputDialog = new DevAddrInputDialog();
    if(pDevAddrInputDialog->exec() == QDialog::Accepted){
        node_addr = pDevAddrInputDialog->get_device_addr();
    }else{
        return ;
    }

    QString firmware;
    QString appVersion;
    qDebug() << "push button scan.....";
    int ret = helper->nodeCheck((CanHelper::DeviceType)node_addr, firmware, appVersion, 50);
    if (ret < 0) {
        outputInformation("没有找到可识别的设备!");
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("没有找到可识别的设备，请检查设备.\t\n");
        msgBox.exec();
        return;
    }

    if(CanHelper::FY169 == node_addr) {
        labelTag->setText("当前选择设备: FY-169仪表  ");
        outputInformation(tr("当前已选择设备: FY-169仪表"));
    } else if(CanHelper::BigDog == node_addr) {
        labelTag->setText("当前选择设备: 大模块  ");
        outputInformation(tr("当前已选择设备: 大模块"));
    }

    if(!firmware.isEmpty()) {
        ui->label_firmwareType->setText(firmware);
        outputInformation(tr("当前固件类型: %1").arg(firmware));
    } else {
        ui->label_firmwareType->setText("未知类型");
        outputInformation(tr("当前固件类型: 未知类型"));
    }
    if(!appVersion.isEmpty()) {
        ui->label_firmwareVersion->setText(appVersion);
        outputInformation(tr("当前固件版本: %1").arg(appVersion));
    } else {
        ui->label_firmwareVersion->setText("未知版本");
        outputInformation(tr("当前固件版本: 未知版本"));
    }
}

void MainWindow::on_updateFirmwarePushButton_clicked()
{
    qDebug() << "update firm ware test .....";
    outputInformation(tr("正在更新固件 ......"));

    //1. 发送扫描节点命令 ， 显示返回内容

    int ret = -1;
    QString firmware, version;

    ret = helper->nodeCheck((CanHelper::DeviceType)node_addr, firmware, version, 50);
    if (ret == -1) {
        qWarning() << "设备扫描失败1";
        outputInformation(tr("设备扫描失败 ......"));
        return;
    }
    qDebug() << "扫描的设备：" << firmware << ", 版本：" << version; //这个显示在界面上
    outputInformation(tr("扫描的设备：固件类型: %1, 版本号: %2").arg(firmware).arg(version));

    // 如果仪表在app层，则发送excuteApp命令， 目的是将仪表转到bootloader层 ---->  上位机发送发送扫描节点命令， 仪表停留在bootloader
    if (firmware == "App" ) {
        ret = helper->excuteApp((CanHelper::DeviceType)node_addr, 0);
        if (ret < 0) {
            qWarning() << "设备跳转失败";
            outputInformation(tr("设备跳转失败 ......"));
            return;
        }

        qDebug() << "跳转到BootLoader层成功"; // 这个要显示在界面上面
        outputInformation(tr("跳转到BootLoader层成功"));

        // 重新扫描设备
        QProgressDialog scanNodeProcess(QStringLiteral("重新扫描节点..."),QStringLiteral("取消"),0,100,this);
        scanNodeProcess.setWindowTitle(QStringLiteral("扫描节点"));
        scanNodeProcess.setModal(true);
        scanNodeProcess.show();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        bool bFind = false;
        int i = 0;
        do{

            ret = helper->nodeCheck((CanHelper::DeviceType)node_addr, firmware, version, 50);

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
        outputInformation(tr("扫描的设备：固件类型: %1, 版本号: %2").arg(firmware).arg(version));
    }

    // 2. 擦除 app
    qDebug() << "before erase flash.";
    ret = helper->eraseFlash((CanHelper::DeviceType)node_addr, 0, 0, 10000);
    if (ret < 0) {
        outputInformation(tr("擦除Flash数据失败！"));
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("警告"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("擦除Flash数据失败 !\t\t\n");
        msgBox.exec();
        return;
    }
    // 3. 读取镜像文件， 发送数据
    QByteArray array;
    array.resize(1026);
    for (int i = 0; i < 1026; i ++) {
        array[i] = 0x00;
    }

    QFile firmwareFile(ui->firmwareLineEdit->text());
    if (!firmwareFile.open(QFile::ReadOnly)) {
        qWarning() << "打开镜像文件失败";
        outputInformation(tr("打开镜像文件失败！"));
        return;
    }
    quint8 FirmwareData[1026]={0};
    int read_data_num;
    QProgressDialog writeDataProcess(QStringLiteral("正在更新固件..."),QStringLiteral("取消"),0,firmwareFile.size(),this);
    writeDataProcess.setWindowTitle(QStringLiteral("更新固件"));
    writeDataProcess.setModal(true);
    writeDataProcess.show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    outputInformation(tr("正在更新固件......"));
    int i=0;
    for(i=0;i<firmwareFile.size();i+=1024){
       read_data_num = firmwareFile.read((char*)FirmwareData,1024);
       uint16_t crc_data = helper->crc16Ccitt(FirmwareData,read_data_num);
       FirmwareData[read_data_num] = crc_data>>8;
       FirmwareData[read_data_num+1] = crc_data;
       read_data_num += 2;
       QByteArray array((const char*)FirmwareData, read_data_num);
       //qDebug() << "array size:" << array.size() << ", read num:" << read_data_num;
       ret = helper->updateFirmware((CanHelper::DeviceType)node_addr, 0, array, 5000);
       if (ret < 0) {
           outputInformation(tr("写Flash数据失败！"));
           QMessageBox msgBox(this);
           msgBox.setWindowTitle(tr("警告"));
           msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText("写Flash数据失败！\t\t\n");
           msgBox.exec();
           return;
       }
       writeDataProcess.setValue(i);
       QCoreApplication::processEvents(QEventLoop::AllEvents);
       if(writeDataProcess.wasCanceled()){
           outputInformation(tr("已取消更新固件"));
           return;
       }
    }

    writeDataProcess.setValue(firmwareFile.size());
    outputInformation(tr("更新固件完成"));
    //QCoreApplication::processEvents(QEventLoop::AllEvents);
    //if(writeDataProcess.wasCanceled()){
    //    return;
    //}
    qDebug() << "ret: " << ret;
    return;
}

void MainWindow::on_executeFirmwarePushButton_clicked()
{
    int ret = -1;
    ret = helper->excuteApp((CanHelper::DeviceType)node_addr, 0);
    if (ret < 0) {
        qWarning() << "设备跳转失败";
        outputInformation(tr("设备跳转失败"));
        return;
    }

    qDebug() << "跳转成功"; // 这个要显示在界面上面
    outputInformation(tr("跳转成功"));
}
