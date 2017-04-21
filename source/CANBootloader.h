/**
  ******************************************************************************
  * @file    CANBootloader.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-CAN�������ײ�������API��������.
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

//ͨ�ô�����
#define	CBL_ERR_SUCCESS					(0)		//û�д���
#define	CBL_ERR_PARAMETER				(-1)	//�������ݴ���
#define	CBL_ERR_OPEN_DEVICE				(-8)	//�豸�򿪴���
#define	CBL_ERR_CLOSE_DEVICE			(-9)	//�豸�رմ���
#define	CBL_ERR_CONFIG					(-20)	//�����豸����
#define	CBL_ERR_DEV_TYPE				(-21)	//�豸���ʹ���
#define	CBL_ERR_SEND					(-22)	//�������ݳ���
#define	CBL_ERR_TIME_OUT				(-23)	//��ʱ����
#define	CBL_ERR_CMD						(-24)	//ִ������ʧ��

typedef  struct  _CBL_CMD_LIST{
	uint8_t EraseFlash;			//����APP������������
	uint8_t	WriteBlockFlash;	//�Զ��ֽ���ʽд����
	uint8_t	ReadBlockFlash;		//�Զ��ֽ���ʽ������
	uint8_t BlockWriteInfo;		//���ö��ֽ�д������ز�����д��ʼ��ַ����������
	uint8_t BlockReadInfo;		//���ö��ֽ����ݶ���ز���������ʼ��ַ����������
    uint8_t	OnlineCheck;		//���ڵ��Ƿ����ߣ�ͬʱ���ع̼���Ϣ
	uint8_t	CmdSuccess;			//����ִ�гɹ�
	uint8_t	CmdFaild;			//����ִ��ʧ��
	uint8_t	SetBaudRate;		//���ýڵ㲨����
	uint8_t	ExcuteApp;			//ִ��Ӧ�ó���
	uint8_t	Undefine0;			//δ����
	uint8_t	Undefine1;			//δ����
	uint8_t	Undefine2;			//δ����
	uint8_t	Undefine3;			//δ����
} CBL_CMD_LIST,*PCBL_CMD_LIST; 

//�����ʼ��CAN����������
typedef struct _INIT_CONFIG_EX
{
    //CAN������ = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
    uint32_t	CAN_BRP;	//ȡֵ��Χ1~1024
    uint8_t		CAN_SJW;	//ȡֵ��Χ1~4
    uint8_t		CAN_BS1;	//ȡֵ��Χ1~16
    uint8_t		CAN_BS2;	//ȡֵ��Χ1~8
    uint8_t		CAN_Mode;	//CAN����ģʽ��0-����ģʽ��1-����ģʽ��2-��Ĭģʽ��3-��Ĭ����ģʽ
    uint8_t		CAN_ABOM;	//�Զ����߹���0-��ֹ��1-ʹ��
    uint8_t		CAN_NART;	//�����ط�����0-ʹ�ܱ����ش���1-��ֹ�����ش�
    uint8_t		CAN_RFLM;	//FIFO��������0-�±��ĸ��Ǿɱ��ģ�1-�����±���
    uint8_t		CAN_TXFP;	//�������ȼ�����0-��ʶ��������1-��������˳�����
    uint8_t		CAN_RELAY;	//�Ƿ����м̹��ܣ�0x00-�ر��м̹��ܣ�0x10-CAN1��CAN2�м̣�0x01-CAN2��CAN1�м̣�0x11-˫���м�
    uint32_t	Reserved;	//ϵͳ����
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
