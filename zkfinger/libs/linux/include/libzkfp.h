#ifndef _libzkfp_h
#define _libzkfp_h
/**
*	@file		libzkfp.h
*	@brief		接口定义
*	@author		scar chen
*	@date		2016-04-12
*	@version	5.0
*	@par	版权：
*				ZKTeco
*	@par	历史版本			
*
*	@note
*
*/


#include "libzkfptype.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define ZKFP_DLOPEN
#ifdef ZKFP_DLOPEN

typedef int (*T_ZKFPM_Init)();
typedef int (*T_ZKFPM_Terminate)();
typedef int (*T_ZKFPM_GetDeviceCount)();
typedef HANDLE (*T_ZKFPM_OpenDevice)(int index);
typedef int (*T_ZKFPM_CloseDevice)(HANDLE hDevice);
typedef int (*T_ZKFPM_SetParameters)(HANDLE hDevice, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);
typedef int (*T_ZKFPM_GetParameters)(HANDLE hDevice, int nParamCode, unsigned char* paramValue, unsigned int* cbParamValue);
typedef int (*T_ZKFPM_AcquireFingerprint)(HANDLE hDevice, unsigned char* fpImage, unsigned int cbFPImage, unsigned char* fpTemplate, unsigned int* cbTemplate);
typedef int (*T_ZKFPM_AcquireFingerprintImage)(HANDLE hDevice, unsigned char* fpImage, unsigned int cbFPImage);

typedef HANDLE (*T_ZKFPM_DBInit)();
typedef int (*T_ZKFPM_DBFree)(HANDLE hDBCache);
typedef int (*T_ZKFPM_DBMerge)(HANDLE hDBCache, unsigned char* temp1, unsigned char* temp2, unsigned char* temp3, unsigned char* regTemp, unsigned int* cbRegTemp);
typedef int (*T_ZKFPM_DBAdd)(HANDLE hDBCache, unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);
typedef int (*T_ZKFPM_DBDel)(HANDLE hDBCache, unsigned int fid);
typedef int (*T_ZKFPM_DBClear)(HANDLE hDBCache);
typedef int (*T_ZKFPM_DBCount)(HANDLE hDBCache, unsigned int* fpCount);
typedef int (*T_ZKFPM_DBIdentify)(HANDLE hDBCache, unsigned char* fpTemplate, unsigned int cbTemplate, unsigned int* FID, unsigned int* score);
typedef int (*T_ZKFPM_DBMatch)(HANDLE hDBCache, unsigned char* template1, unsigned int cbTemplate1, unsigned char* template2, unsigned int cbTemplate2);
typedef int (*T_ZKFPM_ExtractFromImage)(HANDLE hDBCache, const char* lpFilePathName, unsigned int DPI, unsigned char* fpTemplate, unsigned int *cbTemplate);


T_ZKFPM_Init ZKFPM_Init;
T_ZKFPM_Terminate ZKFPM_Terminate;
T_ZKFPM_GetDeviceCount ZKFPM_GetDeviceCount;
T_ZKFPM_OpenDevice ZKFPM_OpenDevice;
T_ZKFPM_CloseDevice ZKFPM_CloseDevice;
T_ZKFPM_SetParameters ZKFPM_SetParameters;
T_ZKFPM_GetParameters ZKFPM_GetParameters;
T_ZKFPM_AcquireFingerprint ZKFPM_AcquireFingerprint;
T_ZKFPM_AcquireFingerprintImage ZKFPM_AcquireFingerprintImage;

T_ZKFPM_DBInit ZKFPM_DBInit;
T_ZKFPM_DBFree ZKFPM_DBFree;
T_ZKFPM_DBMerge ZKFPM_DBMerge;
T_ZKFPM_DBAdd ZKFPM_DBAdd;
T_ZKFPM_DBDel ZKFPM_DBDel;
T_ZKFPM_DBClear ZKFPM_DBClear;
T_ZKFPM_DBCount ZKFPM_DBCount;
T_ZKFPM_DBIdentify ZKFPM_DBIdentify;
T_ZKFPM_DBMatch ZKFPM_DBMatch;
T_ZKFPM_ExtractFromImage ZKFPM_ExtractFromImage;

#else
/**
	*	@brief	初始化库
	*	@param	:
	*		无
	*	@return
	*	返回值说明如下：
	*	value			|	type		|	description of value
	*	----------------|---------------|-------------------------------
	*	0				|	int			|	成功
	*	其他			|	int			|	失败
	*	@note 
*/
ZKINTERFACE int APICALL ZKFPM_Init();

/**
	*	@brief	释放库
	*	@param	:
	*		无
	*	@return
	*	返回值说明如下：
	*	value			|	type		|	description of value
	*	----------------|---------------|-------------------------------
	*	0				|	int			|	成功
	*	其他			|	int			|	失败
	*	@note 
*/
ZKINTERFACE int APICALL ZKFPM_Terminate();

/**
	*	@brief	获取设备数
	*	@param	:
	*		无
	*	@return
	*	返回值说明如下：
	*		当前接入指静脉仪设备数
	*	@note 
*/
ZKINTERFACE int APICALL ZKFPM_GetDeviceCount();

/**
		*	@brief	打开设备
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	index			|	int			  |	[in]				|	设备索引
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	NULL			|	HANDLE		|	失败
		*	其他			|	HANDLE		|	成功
		*	@note 
	*/
	ZKINTERFACE HANDLE APICALL ZKFPM_OpenDevice(int index);

	/**
		*	@brief	关闭设备
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDevice			|	HANDLE		  |	[in]				|	设备操作实例指针
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_CloseDevice(HANDLE hDevice);

	/**
		*	@brief	设置参数
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDevice			|	HANDLE		  |	[in]				|	设备操作实例指针
		*	nParamCode		|	int			  |	[in]				|	参数类型
		*	paramValue		|	unsigned char*|	[in]				|	参数值
		*	cbParamValue	|	unsigned int  |	[in]				|	参数数据长度
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_SetParameters(HANDLE hDevice, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);

	/**
		*	@brief	获取参数
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDevice			|	HANDLE		  |	[in]				|	设备操作实例指针
		*	nParamCode		|	int			  |	[in]				|	参数类型
		*	paramValue		|	unsigned char*|	[out]				|	参数值
		*	cbParamValue	|	unsigned int* |	[out]				|	参数数据长度
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_GetParameters(HANDLE hDevice, int nParamCode, unsigned char* paramValue, unsigned int* cbParamValue);
	
	/**
		*	@brief	获取指纹(图像/模板）
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDevice			|	HANDLE		  |	[in]				|	设备操作实例指针
		*	fpImage			|	unsigned char*|	[out]				|	指纹图像
		*	cbFPImage		|	unsigned int  |	[in]				|	fpImage内存大小
		*	fpTemplate		|	unsigned char*|	[out]				|	指纹模板
		*	cbTemplate		|	unsigned int* |	[in/out]			|	指纹模板长度
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_AcquireFingerprint(HANDLE hDevice, unsigned char* fpImage, unsigned int cbFPImage, unsigned char* fpTemplate, unsigned int* cbTemplate);
		
	/**
		*	@brief	获取指纹图像
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDevice			|	HANDLE		  |	[in]				|	设备操作实例指针
		*	fpImage			|	unsigned char*|	[out]				|	指纹图像
		*	cbFPImage		|	unsigned int  |	[in]				|	fpImage内存大小
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_AcquireFingerprintImage(HANDLE hDevice, unsigned char* fpImage, unsigned int cbFPImage);

	/**
		*	@brief	创建算法操作实例
		*	@param	:
		*	无
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	NULL			|	HANDLE		|	失败
		*	其他			|	HANDLE		|	成功
		*	@note 
	*/
	ZKINTERFACE HANDLE APICALL ZKFPM_DBInit();	

	/**
		*	@brief	释放算法操作实例
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBFree(HANDLE hDBCache); 

	ZKINTERFACE int APICALL ZKFPM_DBSetParameter(HANDLE hDBCache, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);
	ZKINTERFACE int APICALL ZKFPM_DBGetParameter(HANDLE hDBCache, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);


	/**
		*	@brief	将3个指纹模板合成登记模板
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	temp1			|	unsigned char*|	[in]				|	指纹模板1
		*	temp2			|	unsigned char*|	[in]				|	指纹模板2
		*	temp3			|	unsigned char*|	[in]				|	指纹模板3
		*	regTemp			|	unsigned char*|	[out]				|	登记模板
		*	cbRegTemp		|	unsigned int* |	[in/out]			|	登记模板长度
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBMerge(HANDLE hDBCache, unsigned char* temp1, unsigned char* temp2, unsigned char* temp3, unsigned char* regTemp, unsigned int* cbRegTemp);	

	/**
		*	@brief	添加指纹模板到缓存
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	fid				|	unsigned int  |	[in]				|	手指ID
		*	fpTemplate		|	unsigned char*|	[in]				|	指纹模板
		*	cbTemplate		|	unsigned int  |	[in]				|	指纹模板长度
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBAdd(HANDLE hDBCache, unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);

	/**
		*	@brief	从缓存删除指纹模板
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	fid				|	unsigned int  |	[in]				|	手指ID
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBDel(HANDLE hDBCache, unsigned int fid);

	/**
		*	@brief	清空算法缓存
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBClear(HANDLE hDBCache);	

	/**
		*	@brief	获取缓存模板数
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	fpCount			|	unsigned int* |	[out]				|	指纹模板数
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBCount(HANDLE hDBCache, unsigned int* fpCount);	

	/**
		*	@brief	指纹识别(1:N)
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	fpTemplate		|	unsigned char*|	[in]				|	指纹模板
		*	cbTemplate		|	unsigned int  | [in]				|	指纹模板大小
		*	FID				|	unsigned int* |	[out]				|	指静脉ID
		*	score			|	unsigned int* |	[out]				|	分数
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	0				|	int			|	成功
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBIdentify(HANDLE hDBCache, unsigned char* fpTemplate, unsigned int cbTemplate, unsigned int* FID, unsigned int* score);


	/**
		*	@brief	比对两枚指纹
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	template1		|	unsigned char*|	[in]				|	指纹模板1
		*	cbTemplate1		|	unsigned int  | [in]				|	指纹模板1大小
		*	template2		|	unsigned char*|	[in]				|	指纹模板2
		*	cbTemplate2		|	unsigned int  | [in]				|	指纹模板2大小
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	>0				|	int			|	分数
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_DBMatch(HANDLE hDBCache, unsigned char* template1, unsigned int cbTemplate1, unsigned char* template2, unsigned int cbTemplate2);

	/**
		*	@brief	从Bitmap文件提取指纹模板
		*	@param	:
		*	参数说明如下表
		*	name			|	type		  |	param direction		|	description of param
		*	----------------|-----------------|---------------------|------------------------
		*	hDBCache		|	HANDLE		  |	[in]				|	算法操作实例指针
		*	lpFilePathName	|	const char*   |	[in]				|	BMP图片路径
		*	DPI				|	unsigned int  | [in]				|	BMP图片DPI
		*	fpTemplate		|	unsigned char*|	[out]				|	指纹模板
		*	cbTemplate		|	unsigned int* |	[in/out]			|	模板长度
		*	@return
		*	返回值说明如下：
		*	value			|	type		|	description of value
		*	----------------|---------------|-------------------------------
		*	>0				|	int			|	分数
		*	其他			|	int			|	失败
		*	@note 
	*/
	ZKINTERFACE int APICALL ZKFPM_ExtractFromImage(HANDLE hDBCache, const char* lpFilePathName, unsigned int DPI, unsigned char* fpTemplate, unsigned int *cbTemplate);
	
#endif	

#ifdef __cplusplus
};
#endif


#endif	//_libzkfp_h
