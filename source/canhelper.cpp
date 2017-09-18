#include "canhelper.h"
#include <QMessageBox>
#include <QDebug>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QDateTime>
#include "mainwindow.h"

CanHelper::CanHelper(QObject * parent )
    : QObject(parent)
{

}

void CanHelper::attachMainWindow(MainWindow *window)
{
    mWindow = window;
}

// 初始化一次
int CanHelper::open(qint64 canIndex)
{
    if (mInited == TRUE) {
        return 0;
    }
    // open device
    if (VCI_OpenDevice(mDevType, mDevIndex, 0) != STATUS_OK) {
        qWarning() << "Open CAN Device error...";
        //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("Open CAN Device error！"));
        return -1;
    }
    else {
        qDebug() << "Open CAN Device success...";
    }

    // 初始化can1 和 can2
    VCI_INIT_CONFIG InitInfo[1];
    InitInfo->Timing0 = 0x01; // 波特率
    InitInfo->Timing1 = 0x1C;
    InitInfo->Filter  = 0;  // 滤波方式
    InitInfo->AccCode = 0x80000008;
    InitInfo->AccMask = 0xFFFFFFFF;
    InitInfo->Mode=0; // 工作模式是正常状态
    //初始化CAN1
    if(VCI_InitCAN(mDevType, mDevIndex, 0, InitInfo)!=STATUS_OK)	//can-0
    {
        //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("初始化CAN设备Chanel 1失败！"));
        VCI_CloseDevice(mDevType, mDevIndex);
        return -1;
    }
    Sleep(100);
    if(VCI_StartCAN(mDevType, mDevIndex, 0)!=STATUS_OK)	//can-0
    {
        //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("启用CAN设备失败！"));
        VCI_CloseDevice(mDevType, mDevIndex);
        return -1;
    }

    //初始化CAN2
    if(VCI_InitCAN(mDevType, mDevIndex, 1, InitInfo)!=STATUS_OK)	//can-0
    {
        //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("初始化CAN设备Chanel 1失败！"));
        VCI_CloseDevice(mDevType, mDevIndex);
        return -1;
    }
    Sleep(100);
    if(VCI_StartCAN(mDevType, mDevIndex, 1)!=STATUS_OK)	//can-0
    {
        //QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("启用CAN设备失败！"));
        VCI_CloseDevice(mDevType, mDevIndex);
        return -1;
    }
    mCanIndex = canIndex;
    mInited = true;

    return 0;
}

int CanHelper::close()
{
    if (mInited == false) {
        return 0;
    }

    if(VCI_CloseDevice(mDevType, mDevIndex)!=STATUS_OK) {
        return -1;
    }
    mInited = false;
    return 0;
}

int CanHelper::connectState() {
    return VCI_ConnectDevice(mDevType, mDevIndex);
}

int CanHelper::send(quint32 canId, CanFrameFormat format, QByteArray &data)
{
    int ret = -1;
    if (mInited == false) {
        return ret;
    }

    // 获取数据的长度
    int len = data.size();

    int location = 0;
    if (len > 8) {
        do {
            VCI_CAN_OBJ  *can_data_send  =  new  VCI_CAN_OBJ;
            can_data_send->ID = canId;
            can_data_send->SendType = 0;
            can_data_send->DataLen = 8;
            can_data_send->ExternFlag = format;
            can_data_send->RemoteFlag = 0;
            for (int i = 0; i < 8; i++) {
                can_data_send->Data[i] = data[location * 8 + i];
            }
            location += 1;
            len -= 8;
            if ((ret = VCI_Transmit(mDevType, mDevIndex, mCanIndex, can_data_send, 1)) < 0) {
                qWarning() << "send data Failed ...";

            }
            delete(can_data_send);
            can_data_send = NULL;
        } while(len > 8);
    }
    if (len != 0) {
        VCI_CAN_OBJ  *can_data_send  =  new  VCI_CAN_OBJ;
        can_data_send->ID = canId;
        can_data_send->SendType = 0;
        can_data_send->DataLen = len;
        can_data_send->ExternFlag = format;
        can_data_send->RemoteFlag = 0;
        for (int i = 0; i < len; i++) {
            can_data_send->Data[i] = data[location * 8 + i];
        }
        if ((ret = VCI_Transmit(mDevType, mDevIndex, mCanIndex, can_data_send, 1)) < 0) {
            qWarning() << "send data Failed ...";
        }
        delete(can_data_send);
        can_data_send = NULL;
    }
    if(len ==0) {
        VCI_CAN_OBJ  *can_data_send  =  new  VCI_CAN_OBJ;
        can_data_send->ID = canId;
        can_data_send->SendType = 0;
        can_data_send->DataLen = 0;
        can_data_send->ExternFlag = format;
        can_data_send->RemoteFlag = 0;
        if ((ret = VCI_Transmit(mDevType, mDevIndex, mCanIndex, can_data_send, 1)) < 0) {
            qWarning() << "send data Failed ...";
        }
        delete(can_data_send);
        can_data_send = NULL;
    }
    return ret;
}

int CanHelper::nodeCheck(DeviceType dType, QString &firmware,  QString &version, quint32 timeOut)
{

    clearBuffer();
    int ret;
    quint32 canId = (dType << 4) + 1;
    QByteArray array;
    ret = send(canId, CanHelper::ExtendedFrameFormat, array);
    qWarning() << "nodeCheck, send:" << ret;
    if (ret < 0) {
        qWarning() << "in nodeCheck, send data Failed.";
        return ret;
    }

    array.resize(8);
    for (int i = 0; i < 8; ++i) {
        array[i] = 0;
    }
    ret = recv(canId, CanHelper::ExtendedFrameFormat, array, timeOut);
    qDebug() << "nodeCheck, recv ret: "  << ret  ;
    if (ret <= 0) {
        return -1;
    }

    //quint32 appversion = quint32((array[0] << 12) | (array[1] << 8) | (array[2] << 4) | (array[3]));

    quint32 type = quint32(array[7]);
    version.sprintf("fangyan,id:%d,type:%d-v%d.%d", canId, type, ((array[0] << 4 ) | (array[1]) ), ((array[2] << 4) | array[3]));
    if (type == 0) {
        firmware = "Boot";
    }
    else if(type == 1) {
        firmware = "App";
    }
    else {
        firmware = "Nothing";
    }

    return ret;
}

int CanHelper::eraseFlash(CanHelper::DeviceType dType, quint32 flashStartAddr, quint32 flashEndAddr, quint32 timeOut)
{
    clearBuffer();
    int ret;
    quint32 canId = (dType << 4) + 3;
    QByteArray array;

    array[0] = (flashStartAddr >> 24)&0xFF;
    array[1] = (flashStartAddr >> 16) & 0xFF;
    array[2] = (flashStartAddr >> 8) & 0xFF;
    array[3] = flashStartAddr & 0xFF;

    array[4] = (flashEndAddr >> 24)&0xFF;
    array[5] = (flashEndAddr >> 16) & 0xFF;
    array[6] = (flashEndAddr >> 8) & 0xFF;
    array[7] = flashEndAddr & 0xFF;

    ret = send(canId, CanHelper::ExtendedFrameFormat, array);
    if (ret < 0) {
        qWarning() << "in eraseFlash, send data Failed.";
        return ret;
    }

    QByteArray retArray;
    retArray.resize(8);
    for(int i = 0; i < 8; ++i) {
        retArray[i] = 0;
    }
    qWarning() << "in eraseFlash, send data .";
    QProgressDialog scanNodeProcess(QStringLiteral("擦除镜像..."),QStringLiteral("取消"),0,100);
    scanNodeProcess.setWindowTitle(QStringLiteral("镜像擦除"));
    scanNodeProcess.setModal(true);
    scanNodeProcess.show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    bool bFind = false;
    int i = 0;
    int j = 0;
    do{

        ret = recv(canId, CanHelper::ExtendedFrameFormat, retArray, 300); //100ms超时一次
        qDebug() << "in erase flash, ret:" << ret;
        if(ret > 0 ){
            bFind = true;
        }
        if(i>99) {
            i = 0;
            j ++;
        }
        scanNodeProcess.setValue(i);
        i++;
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        if (j >= 2) {
            return -1;
        }
        if(scanNodeProcess.wasCanceled()){
            bFind = true;
            return -1;
        }
    } while(!bFind);
    Q_UNUSED(timeOut);
    return ret;
}

int CanHelper::updateFirmware(CanHelper::DeviceType dType, quint32 flashStartAddr, QByteArray &data, quint32 timeOut)
{
    clearBuffer();
    // 1. 发送writeInfo
    int ret;
    quint32 canId = (dType << 4) + 5;
    QByteArray array;

    array[0] = (flashStartAddr >> 24)&0xFF;
    array[1] = (flashStartAddr >> 16) & 0xFF;
    array[2] = (flashStartAddr >> 8) & 0xFF;
    array[3] = flashStartAddr & 0xFF;

    quint32 len = data.size();
    array[4] = (len >> 24)&0xFF;
    array[5] = (len >> 16) & 0xFF;
    array[6] = (len >> 8) & 0xFF;
    array[7] = len & 0xFF;

    ret = send(canId, CanHelper::ExtendedFrameFormat, array);
    if (ret < 0) {
        qWarning() << "in writeInfo, send data Failed.";
        return ret;
    }

    QByteArray retArray;
    retArray.resize(8);
    for(int i = 0; i < 8; ++i) {
        retArray[i] = 0;
    }
    ret = recv(canId, CanHelper::ExtendedFrameFormat, retArray, timeOut);
    if (ret <= 0) {
        qDebug() << "writeInfo, ret: "  << ret  ;
        return -1;
    }

    // 2. 发送数据给仪表
    clearBuffer();
    quint32 canId2 = (dType << 4) + 6;
    ret = send(canId2, CanHelper::ExtendedFrameFormat, data);
    if (ret < 0) {
        qWarning() << "in write blocking , send data Failed.";
        return ret;
    }

    ret = recv(canId2, CanHelper::ExtendedFrameFormat, retArray, timeOut);
    if (ret <= 0) {
        qDebug() << "write Block, ret: "  << ret  ;
        return -1;
    }

    return ret;
}

int CanHelper::excuteApp(CanHelper::DeviceType dType, quint32 flashStartAddr)
{
    clearBuffer();
    int ret = -1;
    quint32 canId = (dType << 4) + 9;
    QByteArray array;

    array[0] = (flashStartAddr >> 24)&0xFF;
    array[1] = (flashStartAddr >> 16) & 0xFF;
    array[2] = (flashStartAddr >> 8) & 0xFF;
    array[3] = flashStartAddr & 0xFF;


    ret = send(canId, CanHelper::ExtendedFrameFormat, array);
    if (ret < 0) {
        qWarning() << "in writeInfo, send data Failed.";
        return ret;
    }

//    QByteArray retArray;
//    retArray.resize(8);
//    for(int i = 0; i < 8; ++i) {
//        retArray[i] = 0;
//    }
//    ret = recv(canId, CanHelper::ExtendedFrameFormat, retArray, timeOut);
//    if (ret <= 0) {
//        qDebug() << "writeInfo, ret: "  << ret  ;
//        return -1;
//    }

    return ret;
}

/*
 *  Note: 接收一帧数据. 如果没有数据返回，会在超时的时候返回0， 如果有错误会返回-1
 *  私有函数
 */
int CanHelper::recv(quint32 canId, CanHelper::CanFrameFormat format, QByteArray &data, quint32 timeout)
{
    QTime timer;
    timer.start();
    quint32 timeCnt = 0;
    bool bFound = false;
    int recvLen = -1;
    VCI_CAN_OBJ rec[1];
    while(((uint)timer.elapsed()) < timeout) {
        rec[0].ID = 0;
        qDebug() << "recv data:" << recvLen << "," << timeCnt << "," << timeout;
      //  mWindow->outputInformation(tr("CanHelper 数据接收，len： %1, cnt: %2， timeout：%3").arg(recvLen).arg(timeCnt).arg(timeout));
        QCoreApplication::processEvents();
        if ((recvLen=VCI_Receive(mDevType,mDevIndex,mCanIndex,rec,1,1))>0) {

           qDebug() << "recv data:" << recvLen << "," << canId << "," << rec[0].ID;
           if (mWindow->isDebug()) {
               mWindow->outputInformation(tr("已接收到CAN数据，len： %1, canid: %2， recID:%3, cnt:%4, timer's elapesd:%5").arg(recvLen).arg(canId).arg(rec[0].ID).arg(timeCnt).arg(timer.elapsed()));
           }

            if(canId == rec[0].ID) {

                for(int i = 0; i < rec[0].DataLen; ++i) {
                    data[i] = rec[0].Data[i];
                    qDebug() << i << ":" << data.toHex();
                }
                if (mWindow->isDebug()) {
                    mWindow->outputInformation(tr("已接收到目标数据，len： %1, canid: %2， recID:%3, cnt:%4, timer's elapesd:%5").arg(recvLen).arg(canId).arg(rec[0].ID).arg(timeCnt).arg(timer.elapsed()));
                }
                qDebug() << "get data";
                bFound = true;
                timeout = 0;
            }
        }
        Sleep(1);
        timeCnt ++;
    }
//    while(timeCnt < timeout) {
//        rec[0].ID = 0;
//        qDebug() << "recv data:" << recvLen << "," << timeCnt << "," << timeout;
//      //  mWindow->outputInformation(tr("CanHelper 数据接收，len： %1, cnt: %2， timeout：%3").arg(recvLen).arg(timeCnt).arg(timeout));
//        if ((recvLen=VCI_Receive(mDevType,mDevIndex,mCanIndex,rec,1,1))>0) {
//           mWindow->outputInformation(tr("CanHelper 已接收到数据，len： %1, canid: %2， recID:%3, cnt:%4").arg(recvLen).arg(canId).arg(rec[0].ID).arg(timeCnt));
//           qDebug() << "recv data:" << recvLen << "," << canId << "," << rec[0].ID;
//            if(canId == rec[0].ID) {
//                for(int i = 0; i < rec[0].DataLen; ++i) {
//                    data[i] = rec[0].Data[i];
//                    qDebug() << i << ":" << data.toHex();
//                }
//                qDebug() << "get data";
//                bFound = true;
//                timeout = 0;
//            }
//        }
//        Sleep(1);
//        timeCnt ++;
//    }
    Q_UNUSED(format);
    if (!bFound) {
        recvLen = -1;
    }
    return recvLen;
}

void CanHelper::clearBuffer()
{
    VCI_ClearBuffer(mDevType, mDevIndex, mCanIndex);
}

quint16 CanHelper::crc16Ccitt(const quint8 *buf, quint32 len)
{
    static const uint16_t crc16tab[256]= {
        0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
        0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
        0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
        0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
        0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
        0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
        0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
        0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
        0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
        0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
        0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
        0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
        0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
        0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
        0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
        0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
        0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
        0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
        0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
        0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
        0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
        0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
        0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
        0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
        0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
        0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
        0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
        0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
        0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
        0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
        0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
        0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
    };
    register uint32_t counter;
    register uint16_t crc = 0;
    for( counter = 0; counter < len; counter++)
            crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(uint8_t *)buf++)&0x00FF];
    return crc;
}
