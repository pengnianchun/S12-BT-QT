/**
  ******************************************************************************
  * @file    CANBootloader.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-CAN适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef _CANBOOTLOADER_H_
#define _CANBOOTLOADER_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_UNIX
#include <Windows.h>
#endif

//通用错误码
#define	CBL_ERR_SUCCESS					(0)		//没有错误
#define	CBL_ERR_PARAMETER				(-1)	//参数传递错误
#define	CBL_ERR_OPEN_DEVICE				(-8)	//设备打开错误
#define	CBL_ERR_CLOSE_DEVICE			(-9)	//设备关闭错误
#define	CBL_ERR_CONFIG					(-20)	//配置设备错误
#define	CBL_ERR_DEV_TYPE				(-21)	//设备类型错误
#define	CBL_ERR_SEND					(-22)	//发送数据出错
#define	CBL_ERR_TIME_OUT				(-23)	//超时错误
#define	CBL_ERR_CMD						(-24)	//执行命令失败

typedef  struct  _CBL_CMD_LIST{
	uint8_t EraseFlash;			//擦出APP储存扇区数据
	uint8_t	WriteBlockFlash;	//以多字节形式写数据
	uint8_t	ReadBlockFlash;		//以多字节形式读数据
	uint8_t BlockWriteInfo;		//设置多字节写数据相关参数（写起始地址，数据量）
	uint8_t BlockReadInfo;		//设置多字节数据读相关参数（读起始地址，数据量）
    uint8_t	OnlineCheck;		//检测节点是否在线，同时返回固件信息
	uint8_t	CmdSuccess;			//命令执行成功
	uint8_t	CmdFaild;			//命令执行失败
	uint8_t	SetBaudRate;		//设置节点波特率
	uint8_t	ExcuteApp;			//执行应用程序
	uint8_t	Undefine0;			//未定义
	uint8_t	Undefine1;			//未定义
	uint8_t	Undefine2;			//未定义
	uint8_t	Undefine3;			//未定义
} CBL_CMD_LIST,*PCBL_CMD_LIST; 

//定义初始化CAN的数据类型
typedef struct _INIT_CONFIG_EX
{
    //CAN波特率 = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
    uint32_t	CAN_BRP;	//取值范围1~1024
    uint8_t		CAN_SJW;	//取值范围1~4
    uint8_t		CAN_BS1;	//取值范围1~16
    uint8_t		CAN_BS2;	//取值范围1~8
    uint8_t		CAN_Mode;	//CAN工作模式，0-正常模式，1-环回模式，2-静默模式，3-静默环回模式
    uint8_t		CAN_ABOM;	//自动离线管理，0-禁止，1-使能
    uint8_t		CAN_NART;	//报文重发管理，0-使能报文重传，1-禁止报文重传
    uint8_t		CAN_RFLM;	//FIFO锁定管理，0-新报文覆盖旧报文，1-丢弃新报文
    uint8_t		CAN_TXFP;	//发送优先级管理，0-标识符决定，1-发送请求顺序决定
    uint8_t		CAN_RELAY;	//是否开启中继功能，0x00-关闭中继功能，0x10-CAN1到CAN2中继，0x01-CAN2到CAN1中继，0x11-双向中继
    uint32_t	Reserved;	//系统保留
}VCI_INIT_CONFIG_EX,*PVCI_INIT_CONFIG_EX;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI CBL_ScanDevice(uint8_t NeedInit);
int32_t WINAPI CBL_OpenDevice(int32_t DevIndex);
int32_t WINAPI CBL_Config(int32_t DevIndex,int32_t CANIndex,PVCI_INIT_CONFIG_EX pInitConfig,PCBL_CMD_LIST pCmdList);
int32_t WINAPI CBL_NodeCheck(int32_t DevIndex,int32_t CANIndex,uint16_t NodeAddr,uint32_t *pVersion,uint32_t *pType,uint32_t TimeOut);
int32_t WINAPI CBL_EraseFlash(int32_t DevIndex,int32_t CANIndex,uint16_t NodeAddr,uint32_t FlashStartAddr,uint32_t FlashEndAddr,uint32_t TimeOut);
int32_t WINAPI CBL_WriteBlock(int32_t DevIndex,int32_t CANIndex,uint16_t NodeAddr,uint32_t FlashStartAddr,uint8_t *pData,uint32_t DataNum,uint32_t TimeOut);
int32_t WINAPI CBL_ExcuteApp(int32_t DevIndex,int32_t CANIndex,uint16_t NodeAddr,uint32_t FlashStartAddr);
int32_t WINAPI CBL_SetBaudRate(int32_t DevIndex,int32_t CANIndex,uint16_t NodeAddr,PVCI_INIT_CONFIG_EX pInitConfig,uint32_t BaudRate,uint32_t TimeOut);
int32_t WINAPI CBL_CloseDevice(int32_t DevIndex);

#ifdef __cplusplus
}
#endif

#endif
