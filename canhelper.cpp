#include "canhelper.h"
#include <QMessageBox>
#include <QDebug>

CanHelper::CanHelper(QObject * parent )
    : QObject(parent)
{

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
    if (ret < 0) {
        qWarning() << "in nodeCheck, send data Failed.";
        return ret;
    }

    array.resize(8);
    ret = recv(canId, CanHelper::ExtendedFrameFormat, array, timeOut);
    //qDebug() << "nodeCheck, ret: "  << ret  ;
    if (ret <= 0) {
        return -1;
    }

    quint32 appversion = quint32((array[0] << 12) | (array[1] << 8) | (array[2] << 4) | (array[3]));
    version.sprintf("v%d.%d", ((array[0] << 4 ) | (array[1]) ), ((array[2] << 4) | array[3]));
    quint32 type = quint32(array[7]);

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
    ret = recv(canId, CanHelper::ExtendedFrameFormat, retArray, timeOut);
    //    //qDebug() << "eraseFlash, ret: "  << ret  ;
    if (ret < 0) {
        qDebug() << "eraseFlash, ret: "  << ret  ;
        return -1;
    }
//    for(int i = 0; i < 8; ++i) {
//        qDebug()  << i << ":" << (int)retArray[i];
//    }
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
    int ret;
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
    int timeCnt = 0;
    int recvLen = -1;
    VCI_CAN_OBJ rec[1];
    while(timeCnt < timeout) {
        rec[1].ID = 0;
        //qDebug() << "recv data:" << recvLen << "," << timeCnt << "," << timeout;
        if ((recvLen=VCI_Receive(mDevType,mDevIndex,mCanIndex,rec,1,1))>0) {
           qDebug() << "recv data:" << recvLen << "," << canId << "," << rec[0].ID;
            if(canId == rec[0].ID) {
                for(int i = 0; i < rec[0].DataLen; ++i) {
                    data[i] = rec[0].Data[i];
                   // qDebug() << i << ":" << data.toHex();
                }
                qDebug() << "get data";
                timeout = 0;
            }
        }
        Sleep(1);
        timeCnt ++;
    }
    return recvLen;
}

void CanHelper::clearBuffer()
{
    VCI_ClearBuffer(mDevType, mDevIndex, mCanIndex);
}

