#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    helper(new CanHelper)
{
    helper->attachMainWindow(this);
    ui->setupUi(this);
    this->setFixedSize(800, 480);
    this->setWindowTitle(tr("上海方堰CAN固件下载工具 Ver1.0.3"));
    ui->statusBar->showMessage("欢迎使用上海方堰CAN固件下载工具!", 8000);
    //ui->comboBox_selectCan->setCurrentIndex(1);
    ui->label_firmwareType->setText("未知类型");
    ui->label_firmwareVersion->setText("未知版本");
    ui->label_deviceType->setText("未知设备");

    ui->comboBox_selectCan->setCurrentIndex(1);
    ui->comboBox_selectDevice->setCurrentIndex(1);

    labelTag = new QLabel();
    ui->statusBar->addPermanentWidget(labelTag);
    initButtion(false);

    ui->tab_widget->setCurrentIndex(0);

    QFont font("Microsoft YaHei", 12, 45);
    ui->labelCurrentTime->setFont(font);
    //QPalette palette;
    //palette.setColor(QPalette::WindowText,Qt::lightGray);
    ui->milesLineEdit->setPlaceholderText(QStringLiteral("请输入0-9数字"));

    QValidator *validator=new QIntValidator(0,999999,this);
    ui->milesLineEdit->setValidator(validator);
    ui->milesLineEdit->setMaxLength(6);
    ui->milesLineEdit->setAlignment(Qt::AlignRight);
    ui->milesLineEdit->setFont(font);
    ui->label_km->setFont(font);

    ui->tab_widget->setStyleSheet("QTabBar::tab{height:24; width:100}");
    onTimerOut();
    timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

MainWindow::~MainWindow()
{
    if(labelTag!=NULL) {
        delete labelTag;
        labelTag = NULL;
    }
    if(timer!=NULL) {
        timer->stop();
        delete timer;
        timer = NULL;
    }
    helper->close();
    delete helper;
    delete ui;
}

void MainWindow::outputInformation(QString log, QColor rgb) {
    QString temp = "";
    qDebug()<<" --- echo text = "<<log;
    temp.append(QDateTime::currentDateTime().toString("[MM/dd hh:mm:ss:zzz]   "));
    temp.append(log);
    ui->textEdit_outputLog->setTextColor(rgb);
    ui->textEdit_outputLog->append(temp);
}

void MainWindow::on_openFirmwareFileAction_triggered()
{
    on_openFirmwareFilePushButton_clicked();
}


void MainWindow::on_clearDisplayAction_triggered()
{
    ui->textEdit_outputLog->clear();
}

void MainWindow::on_aboutAction_triggered()
{
    QString AboutStr;
    AboutStr = "上海方堰CAN固件下载工具 Ver1.0.3\n";
    AboutStr.append("Shanghai Fangyan 2015-2017 Copyright\n");
    AboutStr.append("Hardware Support: CANalyst-II");
    QMessageBox::about(this,"About CANalyst-II USB-CAN Bootloader",AboutStr);
}

void MainWindow::on_actionHelper_triggered()
{
    QString path;
    QDir dir;
    path=dir.currentPath();
   // QMessageBox::warning(0,"PATH",path,QMessageBox::Yes);
    QString htmlPath = path + "/doc/can.html";
   // bool ret = QDesktopServices::openUrl(QUrl(htmlPath));
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(htmlPath));
    if (ret == false) {
        QMessageBox::warning(0,"PATH",htmlPath + "打开失败",QMessageBox::Yes);
    }
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
    on_pushButton_closeCan_clicked();
}

void MainWindow::on_pushButton_openCan_clicked()
{
    QString tipsInfo;
    if ( -1 == helper->open(m_nCanIndex) ) {
        tipsInfo = "CAN设备打开失败 !";
        ui->statusBar->showMessage(tipsInfo, 3000);
        outputInformation(tipsInfo, Qt::red);
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("CAN设备打开失败 !\t\t\n");
        msgBox.exec();
        return;
    }
    tipsInfo = "CAN设备打开成功 !";

    //将一些按钮灰度化
    ui->pushButton_openCan->setEnabled(false); //打开设备
    ui->pushButton_closeCan->setEnabled(true); //关闭设备
    ui->pushButton_openDevice->setEnabled(true); // 连接设备
    ui->updateFirmwarePushButton->setEnabled(false); //更新固件
    ui->executeFirmwarePushButton->setEnabled(false); //执行固件

    ui->statusBar->showMessage(tipsInfo, 3000);
    outputInformation(tipsInfo);

    displayFlag = true;
    stepCount = 0;
}

void MainWindow::on_pushButton_closeCan_clicked()
{
    ui->pushButton_openCan->setEnabled(true);
    ui->pushButton_closeCan->setEnabled(false);
    QString tipsInfo;
    if (-1 == helper->close()) {
        tipsInfo = "CAN设备关闭失败 !";
        ui->statusBar->showMessage(tipsInfo, 3000);
        return;
    }

    tipsInfo = "CAN设备已关闭 !";

    //将一些按钮灰度化
    ui->pushButton_openCan->setEnabled(true); //打开设备
    ui->pushButton_closeCan->setEnabled(false); //关闭设备
    ui->pushButton_openDevice->setEnabled(false); // 连接设备
    ui->updateFirmwarePushButton->setEnabled(false); //更新固件
    ui->executeFirmwarePushButton->setEnabled(false); //执行固件

    ui->statusBar->showMessage(tipsInfo, 3000);
    outputInformation(tipsInfo);

    displayFlag = true;
    stepCount = 0;
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


void MainWindow::on_updateFirmwarePushButton_clicked()
{
    qDebug() << "update firm ware test .....";
    outputInformation(tr("正在更新固件 ......"));

    //1. 发送扫描节点命令 ， 显示返回内容

    int ret = -1;
    QString firmware, version;

    ret = helper->nodeCheck(m_nodeAddr, firmware, version, 50);
    if (ret == -1) {
        qWarning() << "设备扫描失败1";
        outputInformation(tr("设备扫描失败 ......"));
        return;
    }
    qDebug() << "扫描的设备：" << firmware << ", 版本：" << version; //这个显示在界面上
    outputInformation(tr("扫描的设备：固件类型: %1, 版本号: %2").arg(firmware).arg(version));
    displaylabelTag(firmware, version);
    // 如果仪表在app层，则发送excuteApp命令， 目的是将仪表转到bootloader层 ---->  上位机发送发送扫描节点命令， 仪表停留在bootloader
    if (firmware == "App" ) {
        ret = helper->excuteApp(m_nodeAddr, 0);
        if (ret < 0) {
            qWarning() << "设备跳转失败";
            outputInformation(tr("设备跳转失败 ......"));
            return;
        }

        qDebug() << "跳转到BootLoader层成功"; // 这个要显示在界面上面
        outputInformation(tr("跳转到BootLoader层成功"));
        Sleep(500); // 等待一段时间
        // 重新扫描设备
        QProgressDialog scanNodeProcess(QStringLiteral("重新扫描节点..."),QStringLiteral("取消"),0,100,this);
        scanNodeProcess.setWindowTitle(QStringLiteral("扫描节点"));
        scanNodeProcess.setModal(true);
        scanNodeProcess.show();
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        bool bFind = false;
        int i = 0;
        do{

            ret = helper->nodeCheck(m_nodeAddr, firmware, version, 50);

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
        displaylabelTag(firmware, version);
        outputInformation(tr("扫描的设备：固件类型: %1, 版本号: %2").arg(firmware).arg(version));
    }

    // 2. 擦除 app
    qDebug() << "before erase flash.";
    ret = helper->eraseFlash(m_nodeAddr, 0, 0, 10000);
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
       ret = helper->updateFirmware(m_nodeAddr, 0, array, 5000);
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
    QString oldFirmware = m_firmware;
    QString oldVersion = m_version;
    int ret = -1;
    ret = helper->excuteApp(m_nodeAddr, 0);
    Sleep(500); //先等待一段时间，目的是让设备跳过去
    ret = scanNode("跳转");
    if (ret < 0) {
        qWarning() << "设备跳转失败";
        outputInformation(tr("设备跳转失败"));
        return;
    }

    //
    if (oldFirmware == "App") { //如果以前是app
        if(m_firmware != "Boot") {
            qWarning() << "设备跳转失败";
            outputInformation(tr("由App跳转到Boot失败"));
            return;
        }
    }
    if (oldFirmware == "Boot") {
        if (m_firmware != "App") {
            qWarning() << "设备跳转失败";
            outputInformation(tr("由Boot调转到App失败"));
            return;
        }
    }

    qDebug() << "跳转成功"; // 这个要显示在界面上面
    outputInformation(tr("跳转成功"));

}

void MainWindow::on_comboBox_selectDevice_currentIndexChanged(int index)
{

    if(0 == index) {
        m_nodeAddr = CanHelper::FY169;
    } else if(1 == index){
        m_nodeAddr = CanHelper::FY169;
        outputInformation(tr("当前已选择FY169仪表"));
    } else {
        m_nodeAddr = CanHelper::BigDog;
        outputInformation(tr("当前已选择大模块"));
    }
    //将一些按钮灰度化
    ui->updateFirmwarePushButton->setEnabled(false); //更新固件
    ui->executeFirmwarePushButton->setEnabled(false); //执行固件
}

//连接设备
void MainWindow::on_pushButton_openDevice_clicked()
{

    //将一些按钮灰度化
    ui->updateFirmwarePushButton->setEnabled(false); //更新固件
    ui->executeFirmwarePushButton->setEnabled(false); //执行固件

    int ret = scanNode(tr("连接设备"));
    if (ret < 0) {
        outputInformation("没有找到可识别的设备!");
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("没有找到可识别的设备，请检查设备.\t\n");
        msgBox.exec();

        //将一些按钮灰度化
        ui->updateFirmwarePushButton->setEnabled(false); //更新固件
        ui->executeFirmwarePushButton->setEnabled(false); //执行固件
    }
    else {
        //将一些按钮灰度化
        ui->updateFirmwarePushButton->setEnabled(true); //更新固件
        ui->executeFirmwarePushButton->setEnabled(true); //执行固件
    }
}

void MainWindow::displaylabelTag(QString &firmware, QString &version)
{
    QString labelTagStr;
    if(CanHelper::FY169 == m_nodeAddr) {
        labelTagStr.append("当前选择设备: FY-169仪表  ");
        ui->label_deviceType->setText("FY-169仪表");
        outputInformation(tr("当前已选择设备: FY-169仪表"));
    } else if(CanHelper::BigDog == m_nodeAddr) {
        labelTagStr.append("当前选择设备: 大模块  ");
        ui->label_deviceType->setText("大模块");
        outputInformation(tr("当前已选择设备: 大模块"));
    }

    if(!firmware.isEmpty()) {
        ui->label_firmwareType->setText(firmware);
        labelTagStr.append(tr("当前固件类型: %1   ").arg(firmware));
        outputInformation(tr("当前固件类型: %1").arg(firmware));
    } else {
        ui->label_firmwareType->setText("未知类型");
        labelTagStr.append(tr("当前固件类型: 未知类型   "));
        outputInformation(tr("当前固件类型: 未知类型"));
    }
    if(!version.isEmpty()) {
        ui->label_firmwareVersion->setText(version);
        labelTagStr.append(tr("当前固件版本: %1 ").arg(version));
        outputInformation(tr("当前固件版本: %1").arg(version));
    } else {
        ui->label_firmwareVersion->setText("未知版本");
        labelTagStr.append(tr("当前固件版本: 未知版本 "));
        outputInformation(tr("当前固件版本: 未知版本"));
    }
    labelTag->setText(labelTagStr);
    m_firmware = firmware;
    m_version = version;
}

int MainWindow::scanNode(const QString &text)
{
    QString firmware;
    QString version;
    displaylabelTag(firmware, version);
    //
    QProgressDialog scanNodeProcess(text + "中...",QStringLiteral("取消"),0,100,this);
    scanNodeProcess.setWindowTitle(text);
    scanNodeProcess.setModal(true);
    scanNodeProcess.show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    bool bFind = false;
    int i = 0;
    int j = 0;
    int ret = -1;
    do{

        ret = helper->nodeCheck(m_nodeAddr, firmware, version, 50);
        qWarning() << "in scanNode : " << ret;
        if(ret >= 0 ){
            bFind = true;
        }
        if(i>99) {
            i = 0;
            j++;
        }
        if (j >= 3) { //循环了3次，说明没有设备连接
            displaylabelTag(firmware, version);
            return -1;
        }
        scanNodeProcess.setValue(i);
        i++;
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        if(scanNodeProcess.wasCanceled()){
            bFind = true;
            displaylabelTag(firmware, version);
            return -1;
        }
    } while(!bFind);

    displaylabelTag(firmware, version);
    return 0;
}

void MainWindow::initButtion(bool inited)
{
    //将一些按钮灰度化
    ui->pushButton_openCan->setEnabled(!inited); //打开设备
    ui->pushButton_closeCan->setEnabled(inited); //关闭设备
    ui->pushButton_openDevice->setEnabled(inited); // 连接设备
    ui->updateFirmwarePushButton->setEnabled(inited); //更新固件
    ui->executeFirmwarePushButton->setEnabled(inited); //执行固件

}

void MainWindow::on_pushButton_sync_clicked()
{
    QByteArray array;
    QString dataBuffer = "136 136";
    QString dateString = QDateTime::currentDateTime().toString(" hh mm yy MM dd ");
    dataBuffer.append(dateString);
    dataBuffer.append("00");
    qDebug()<<" --- data buffer = "<<dataBuffer;
    QStringList list;
    list.clear();
    list = dataBuffer.split(" ");

    qint64 temp = 0;
    for(int i=0; i<2; i++) {
        QString dat = list.at(i);
        temp = dat.toInt();
        array[i] = temp;
    }

    for(int i=2; i<8; i++) {
        QString dat = list.at(i);
        temp = dat.toInt();
        array[i] = ((temp/10)&0x0F)<<4;
        array[i] = array[i]|((temp%10)&0x0F);
    }
    qint64 flag = helper->send(0x00000888, CanHelper::ExtendedFrameFormat, array);
    if(flag<1)
    {
        //ui->labelTips->setText(tr("同步时间失败"));
        outputInformation(tr("同步时间失败"),Qt::red);
        displayFlag = true;
        stepCount = 0;
        if(flag==-1){
            //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("设备打开失败"));
            return;
        } else if(flag==0){
            //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("发送报文失败"));
            return;
        }
    } else {
        if(1 == flag) {
            outputInformation(tr("同步时间成功,请检查是否已更新"),Qt::green);
            //ui->labelTips->setText(tr("同步时间成功,请检查是否已更新"));
        } else {
            outputInformation(tr("同步时间失败"),Qt::red);
            //ui->labelTips->setText(tr("同步时间失败"));
        }
        displayFlag = true;
        stepCount = 0;
        qDebug()<<" --- send message result = "<<flag;
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents);
}

void MainWindow::onTimerOut() {
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->labelCurrentTime->setText(str);
    if(true == displayFlag) {
        stepCount++;
        if(stepCount>5) {
            displayFlag = false;
            stepCount = 0;
        }
    }
    deviceState = helper->connectState();
    if(0 == deviceState) {
        helper->close();
        ui->pushButton_sync->setEnabled(false);
    }
}

void MainWindow::on_pushButton_WriteMiles_clicked()
{
    if(ui->milesLineEdit->text().toInt() <=0){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("警告"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("请输入里程数！\t\t\n");
        msgBox.exec();
        return;
    } else {
        writeMile = ui->milesLineEdit->text().toInt();
    }

    QMessageBox msgBox(this);
    msgBox.setFixedWidth(300);
    msgBox.setWindowTitle(tr("提示"));
    msgBox.setText(tr("\t\t\n你确定要写入当前里程:  %1  \t\n").arg(QString::number(writeMile)));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if(QMessageBox::No == ret) {
        outputInformation(tr("当前里程尚未写入!"));
        return;
    }

    //写入里程 Todo code here
    if(QMessageBox::Yes == ret)
    {
        QString Mileage = ui->milesLineEdit->text();
        long int IntMileage=Mileage.toInt();
        QByteArray array;
        QStringList list = Mileage.split("");
       // array=Mileage.toInt();
        list.append(Mileage);
        //int IntMileage = Mileage.toInt();
        array[0]=0x04;
        for(int i = 1;i<5;i++)
        {
            array[i]=(IntMileage&0xff);
            IntMileage=IntMileage>>8;
        }
  //      outputInformation(array,Qt::green);
        qint64 flag = helper->send(0x00000610, CanHelper::ExtendedFrameFormat, array);
        if(1 == flag) {
            outputInformation(tr("里程修改成功,请检查是否已更新"),Qt::green);
            //ui->labelTips->setText(tr("同步时间成功,请检查是否已更新"));
        } else {
            outputInformation(tr("里程修改失败"),Qt::red);
            //ui->labelTips->setText(tr("同步时间失败"));
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}
