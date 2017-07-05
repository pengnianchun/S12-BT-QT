#ifndef CANHELPER_H
#define CANHELPER_H

#include <QObject>
#include <QThread>
#include "controlcan.h"

class CanHelper : public QObject
{
    Q_OBJECT
public:
    explicit CanHelper(QObject *parent = 0);

public:
    enum CanFrameFormat {
        StandardFrameFormat = 0,
        ExtendedFrameFormat
    };

    enum DeviceType {
        FY169 = 0x12,
        BigDog = 0x13
    };

public:
    int open(qint64 canIndex);

    int close();

    int send(quint32 canId, CanFrameFormat format, QByteArray &data);

    /*
     *  烧写bootloader相关. 设备选择相关
     *  dType: 设备类型,
     *  version[OUT]: 版本信息
     *  firmware[OUT]: 固件类型 Boot, App
     */
    int nodeCheck(DeviceType dType, QString &firmware,  QString &version, quint32 timeOut);

    int eraseFlash(DeviceType dType, quint32 FlashStartAddr,quint32 FlashEndAddr,quint32 TimeOut);

    int updateFirmware(DeviceType dType, quint32 flashStartAddr, QByteArray &data, quint32 timeOut);

    int excuteApp(DeviceType dType, quint32 flashStartAddr);

private:
    /*
     * Note: 数据接收函数。 接收一帧数据
     */
    int recv(quint32 canId, CanFrameFormat format, QByteArray &data, quint32 timeout);

    void clearBuffer();
signals:

protected:
    void run();



private:
    bool mInited = false;
    DWORD mDevType = VCI_USBCAN2; // 适配器设备类型定义 ,  这个一般不用动
    qint64 mDevIndex = 0; // 设备索引，比如当只有一个USB-CAN适配器时，索引号为0，这时再插入一个USB-CAN适配器那么后面插入的这个设备索引号就是1，以此类推
    qint64 mCanIndex = 0; // can索引号： 0-> CAN1, 1-> CAN2

};

#endif // CANHELPER_H
