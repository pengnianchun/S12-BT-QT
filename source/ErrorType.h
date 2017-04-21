#ifndef _ERRORTYPE_H_
#define _ERRORTYPE_H_

#include <stdint.h>
#ifndef OS_LINUX
#include <Windows.h>
#endif

//通用错误码
#define	ERR_SUCCESS					(0)		//没有错误
#define	ERR_PARAMETER_NULL			(-1)	//传入的指针为空指针
#define	ERR_INPUT_DATA_TOO_MUCH		(-2)	//参数输入个数多余规定个数
#define	ERR_INPUT_DATA_TOO_LESS		(-3)	//参数输入个数少余规定个数
#define	ERR_INPUT_DATA_ILLEGALITY	(-4)	//参数传入格式和规定的不符合
#define	ERR_USB_WRITE_DATA			(-5)	//USB写数据错误
#define	ERR_USB_READ_DATA			(-6)	//USB读数据错误
#define	ERR_READ_NO_DATA			(-7)	//请求读数据时返回没有数据
#define	ERR_OPEN_DEVICE				(-8)	//打开设备失败
#define	ERR_CLOSE_DEVICE			(-9)	//关闭设备失败
#define	ERR_EXECUTE_CMD				(-10)	//设备执行命令失败
#define ERR_SELECT_DEVICE			(-11)	//选择设备失败
#define	ERR_DEVICE_OPENED			(-12)	//设备已经打开
#define	ERR_DEVICE_NOTOPEN			(-13)	//设备没有打开
#define	ERR_BUFFER_OVERFLOW			(-14)	//缓冲区溢出
#define	ERR_DEVICE_NOTEXIST			(-15)	//此设备不存在
#define	ERR_LOAD_KERNELDLL			(-16)	//装载动态库失败
#define ERR_CMD_FAILED				(-17)	//执行命令失败错误码
#define	ERR_BUFFER_CREATE			(-18)	//内存不足

//ULONG VCI_Transmit(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PVCI_CAN_OBJ pSend, ULONG Len); 其中PVCI_CAN_OBJ是一个结构，
//typedef struct _VCI_CAN_OBJ {
//    UINT ID;
//    UINT TimeStamp;
//    BYTE TimeFlag;
//    BYTE SendType;
//    BYTE RemoteFlag;
//    BYTE ExternFlag;
//    BYTE DataLen;
//    BYTE Data[8];
//    BYTE Reserved[3];
//} VCI_CAN_OBJ, *PVCI_CAN_OBJ;

#ifdef __cplusplus
extern "C"
{
#endif
void WINAPI VTI_GetErrorString(int32_t ErrorValue,int8_t* pErrorString);
//ULONG WINAPI VCI_Transmit(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PVCI_CAN_OBJ pSend, ULONG Len);
#ifdef __cplusplus
}
#endif

#endif
