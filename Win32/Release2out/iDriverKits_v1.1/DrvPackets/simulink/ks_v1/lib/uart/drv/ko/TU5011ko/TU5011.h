#ifndef __DLL_TU5011_API_H__
#define __DLL_TU5011_API_H__

#ifdef __cplusplus
extern "C"{
#endif
#ifndef __PS_DEVICE_COMMON_DEFINE__
#define __PS_DEVICE_COMMON_DEFINE__

#define MAX_DEVICE_ID_LEN	200

#ifndef __PANSINO_SOLUTIONS_UNSIGNED_INT_32_TYPE_DEFINED__
#define __PANSINO_SOLUTIONS_UNSIGNED_INT_32_TYPE_DEFINED__
#if defined(__linux__) || defined(VXWORKS)
typedef unsigned int	uInt32;
#else
typedef unsigned long	uInt32;
#endif
#endif

typedef void* PS_DevObject;


/// �豸��Ϣ���
typedef PS_DevObject PS_DevInfoHandle;

/// �豸���
typedef PS_DevObject PS_DevHandle;

/// �豸����
typedef enum
{
	ATTR_MANF_ID	=	0,			///< VendorID				__[unsigned short]
	ATTR_MODEL_CODE,				///< DeviceID				__[unsigned short]
	ATTR_SLOT,						///< slot					__[short]
	ATTR_PXI_CHASSIS,				///< chassis				__[short]
	ATTR_PXI_DEV_NUM,				///< device number			__[unsigned short]
	ATTR_PXI_FUNC_NUM,				///< function number		__[unsigned short]
	ATTR_PXI_BUS_NUM,				///< bus number				__[unsigned short]
	ATTR_RSRC_NAME					///< address string			__[char*]

}ATTRIBUTE;

/// ��������״̬
enum RETURNERRORS
{
	PS_SUCCESS = 0,					///< �����ɹ�:0
	PS_FAILED,						///< �������ɹ�:1
	PS_UNKNOWN_STATUS,				///< δ֪״̬:2
	PS_ERROR_STATUS,				///< ����״̬:3
	PS_INVALID_PARAM,				///< ��Ч����ֵ:4

	PS_TIME_OUT,					///< ������ʱ:5

	PS_ERR_OPEN_KERNEL,				///< �򿪺����豸ʧ��:6
	PS_ERR_CLOSE_KERNEL,			///< �رպ����豸ʧ��:7

	PS_RCV_BUFFER_EMPTY,			///< ���ջ���Ϊ��:8
	PS_RCV_BUFFER_FULL,				///< ���ջ�������:9
	PS_SND_BUFFER_EMPTY,			///< ���ͻ���Ϊ��:10
	PS_SND_BUFFER_FULL,				///< ���ͻ�������:11

	PS_DEV_BUSY,					///< �豸æ:12
	PS_DEV_INVALID_STATUS,			///< �豸��Ч״̬���ò������豸״̬����Ч:13

	PS_DANGER_DEV_INPUT,			///< �豸���ⲿ�����Σ���豸��ȫ:14
	PS_WAIT_EVENT_CANCELLED,		///< �˳��ȴ��¼�:15

	PS_DATA_IS_NOT_ENOUGH,			///< ����������:16
	PS_SPACE_IS_NOT_ENOUGH,			///< �ռ䲻��:17

	PS_TRANSMIT_TIME_OUT,			///< ���ݷ��ͳ�ʱ:18

	PS_ERROR_SRAM,					///< �豸SRAM����:19
	PS_ERROR_LOOP					///< ���罨��ʧ��:20
};

#endif

#define ErrorNotSetupEventHndlr									(-10000)				///< ʹ���¼�ʧ��
#define ErrorNotOpenMemacc										(-10001)				///< �������ڴ�ʧ��
#define ErrorNotAllocPhyMemory									(-10002)				///< ���������ڴ�ʧ��

typedef enum _ParityBit
{
	Bit0 = 0x00000000,						/// 0У�飬��ʾ��������Ϊ���٣���λΪ0
	Bit1 = 0x00000010,						/// 1У�飬��ʾ��������Ϊ���٣���λΪ1
	Odd  = 0x00000020,						/// ��У��
	Even = 0x00000030,						/// żУ��
}ParityBit,*PParityBit;

typedef enum _StopBit
{
	stop1   = 0x00000000,					/// ֹͣλΪ1
	stop1_5 = 0x00000100,					/// ֹͣλΪ1.5
	stop2   = 0x00000200,					/// ֹͣλΪ2
}StopBit, *PStopBit;

typedef enum _DataBit
{
	Data5 = 0x00000001,						/// ���ݳ���λΪ5
	Data6 = 0x00000002,						/// ���ݳ���λΪ6
	Data7 = 0x00000003,						/// ���ݳ���λΪ7
	Data8 = 0x00000000,						/// ���ݳ���λΪ8
}DataBit, *PDataBit;

typedef enum _SerialMode
{
	RS232 = 0x00000000,						/// ����ģʽRS232
	RS422 = 0x00001000,						/// ����ģʽRS422
	RS485 = 0x00002000,						/// ����ģʽRS485
}SerialMode, *PSerialMode;

typedef enum _InterType
{
	TxInter = 0,							/// �����ж�
	RxInter,								/// �����ж�
	OverflowInter,							/// ��������ж�
}InterType, *PInterType;

typedef struct _InterMode
{
	int		        channel;				/// ͨ����
	int		        threshold;				/// ����ֵ
	InterType		intertype;				/// �ж�����
}InterMode, *PInterMode;

typedef struct _AsyncChannelMode
{		
	ParityBit				parityBit;			/// У��λ
	StopBit				stopBit;			/// ֹͣλ
	DataBit				lengthBit;			/// ���ݳ���λ
	SerialMode         	 	serialMode;			/// ����ģʽ
	int					ParityEnable;		/// У��ʹ�ܣ�Ϊ1ʹ��
	int					TermiResistor;		/// ����ƥ����裬Ϊ1���ã�����ͨ��ʱ����Ҫһ·����ͨ������ƥ�����
	int					ChooseWrongData;	/// У��������ݱ�����Ϊ1����
	uInt32		baudrate;				/// ����������
}AsyncChannelMode, *PAsyncChannelMode;

/// ɨ��PTU5011�豸
int TU5011_ScanDevInfo(PS_DevInfoHandle* infoHandle, int *pnCnt, char srcName[]);

/// ������һ���豸��Ϣ
int TU5011_FindNextDevInfo(PS_DevInfoHandle infoHandle, char srcName[]);

/// �ͷ��ͷ�PTU5011�豸��Ϣ��
int TU5011_ReleaseDevice(PS_DevInfoHandle infoHandle);

/// ��PTU5011�豸���
int TU5011_OpenDevice(PS_DevInfoHandle infoHandle, PS_DevHandle *phDev);

/// ���ݲ�λ�򿪰忨
int TU5011_OpenDeviceBySlot(short chassis, short slot, PS_DevHandle* phDev);

/// �����豸��Դ���ƴ򿪰忨
int TU5011_OpenDeviceByName(char srcName[], PS_DevHandle* phDev);

/// �ر�PTU5011�豸
int TU5011_CloseDevice(PS_DevHandle hDev);

/// ����豸����
int TU5011_GetAttribute(PS_DevHandle hDev, ATTRIBUTE attribute, void *attrState);

/// ���FPGA�汾��Ϣ
int TU5011_GetFPGAVersionn(PS_DevHandle hDev, uInt32* version);

/// ��λ�忨
int TU5011_Reset(PS_DevHandle hDev);

/// ��������־
int TU5011_ClearOverflowIndicator(PS_DevHandle hDev, int channel);

/// �����첽ͨ��ģʽ
int TU5011_SetAsyncChannelMode(PS_DevHandle hDev, int channel, AsyncChannelMode Mode);

/// ��ȡ�첽���͸���
int TU5011_GetCountOfTxData(PS_DevHandle hDev, int channel, uInt32* count);

/// ��ȡ�첽���ո���
int TU5011_GetCountOfRxData(PS_DevHandle hDev, int channel, uInt32* count);

/// ʹ�ܷ���
int TU5011_EnableSend(PS_DevHandle hDev, int channel, int Enable);

/// ʹ�ܽ���
int TU5011_EnableReceive(PS_DevHandle hDev, int channel, int Enable);

/// ��շ���FIFO
int TU5011_ClearSendFIFO(PS_DevHandle hDev, int channel);

/// ��ս���FIFO
int TU5011_ClearReceiveFIFO(PS_DevHandle hDev, int channel);

/// ��������
int TU5011_LoadAsyncData(PS_DevHandle hDev, int channel, unsigned char* pBuffer, uInt32 lengthToWrite, uInt32 *lengthWritten);

/// ��ȡ����
int TU5011_ReadAsyncData(PS_DevHandle hDev, int channel, unsigned char* pBuffer, uInt32 lengthToRead, uInt32 *lengthRead);

/// �����Լ�ģʽʹ��
int TU5011_SelfTestEnable(PS_DevHandle hDev, uInt32 enable);

/// �����Լ�ͨ��
int TU5011_SetSelfTestChannel(PS_DevHandle hDev, int TxChannel, int RxChannel);

/// ˢ�¹���
int TU5011_RefreshFunction(PS_DevHandle hDev, uInt32 enable);

/// �����ϵ�״̬
int TU5011_SetPowerState(PS_DevHandle hDev, uInt32 State);

/// ��ȡ��ͨ���ϵ�״̬ʱ��ͨ������
int TU5011_GetPowerStateOfChannel(PS_DevHandle hDev, int channel, AsyncChannelMode* Mode);

/// ********************************Interrupt Operation ***********************************
/// �ȴ��ж�
int TU5011_WaitForInterrupt(PS_DevHandle hDev, int interval);

/// ȡ���ж�
int TU5011_CancelWaitForInterrupt(PS_DevHandle hDev);

/// �����ж�
int TU5011_DisableInterrupt(PS_DevHandle hDev, int interLength, InterMode* intermode);

/// �����ж�ģʽ
int TU5011_SetInterruptMode(PS_DevHandle hDev, int interLength, InterMode* interMode);

/// ��ȡ�ж�ģʽ
int TU5011_GetInterruptMode(PS_DevHandle hDev, int interLength, InterMode* interMode, int* lengthSet);

/// ����жϸ���
int TU5011_GetInterLength(PS_DevHandle hDev, int* interLength);

#ifdef __cplusplus
}
#endif
#endif

