#ifndef ZKFINGER_H
#define ZKFINGER_H

/**
 *	@file		zkfinger.h
 *	@brief		指纹仪库函数接口
 */

#include "libzkfptype.h"
#include "libzkfperrdef.h"

#ifndef _WIN32
#include <dlfcn.h>
#define	GetProcAddress(x, y) dlsym((x), (y))
#define HMODULE void *
#define FreeLibrary dlclose
#define LoadLibraryA(a) dlopen(a, RTLD_LAZY)
#else
#include <windows.h>
#endif

#ifdef _WIN32
#define ZKFP_LIB_NAME "libzkfp.dll"
#else
#define ZKFP_LIB_NAME "libzkfp.so"
#endif


typedef int (APICALL *FPSensorInit)();
typedef int (APICALL *FPSensorTerminate)();
typedef int (APICALL *FPSensorGetDeviceCount)();
typedef void* (APICALL *FPSensorOpenDevice)(int index);
typedef int (APICALL *FPSensorCloseDevice)(void * hDevice);
typedef int (APICALL *FPSensorSetParameters)(void * hDevice, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);
typedef int (APICALL *FPSensorGetParameters)(void * hDevice, int nParamCode, unsigned char* paramValue, unsigned int* cbParamValue);
typedef int (APICALL *FPSensorAcquireFingerprint)(void * hDevice, unsigned char* fpImage, unsigned int cbFPImage, unsigned char* fpTemplate, unsigned int* cbTemplate);
typedef int (APICALL *FPSensorAcquireFingerprintImage)(void * hDevice, unsigned char* fpImage, unsigned int cbFPImage);

typedef void* (APICALL *FPSensorDBInit)();
typedef int (APICALL *FPSensorDBFree)(void * hDBCache);
typedef int (APICALL *FPSensorDBMerge)(void * hDBCache, unsigned char* temp1, unsigned char* temp2, unsigned char* temp3, unsigned char* regTemp, unsigned int* cbRegTemp);
typedef int (APICALL *FPSensorDBAdd)(void * hDBCache, unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);
typedef int (APICALL *FPSensorDBDel)(void * hDBCache, unsigned int fid);
typedef int (APICALL *FPSensorDBClear)(void * hDBCache);
typedef int (APICALL *FPSensorDBCount)(void * hDBCache, unsigned int* fpCount);
typedef int (APICALL *FPSensorDBIdentify)(void * hDBCache, unsigned char* fpTemplate, unsigned int cbTemplate, unsigned int* FID, unsigned int* score);
typedef int (APICALL *FPSensorDBMatch)(void * hDBCache, unsigned char* template1, unsigned int cbTemplate1, unsigned char* template2, unsigned int cbTemplate2);
typedef int (APICALL *FPSensorExtractFromImage)(void * hDBCache, const char* lpFilePathName, unsigned int DPI, unsigned char* fpTemplate, unsigned int *cbTemplate);
typedef int (APICALL *FPSensorBase64ToBlob)(const char* src, unsigned char* blob, unsigned int cbBlob);
typedef int (APICALL *FPSensorBlobToBase64)(const unsigned char* src, unsigned int cbSrc, char* base64Str, unsigned int cbBase64str);
typedef int (APICALL *FPSensorDBSetParameter)(void * hDBCache, int nParamCode, int paramValue);
typedef int (APICALL *FPSensorDBGetParameter)(void * hDBCache, int nParamCode, int* paramValue);

typedef int (APICALL *FPSensorVerifyByID)(void * hDBCache, unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);
typedef int (APICALL *FPSensorGetCaptureParams)(void * hDevice, PZKFPCapParams pCapParams);
typedef int (APICALL *FPSensorGetCaptureParamsEx)(void * hDevice, int* width, int* height, int* dpi);
typedef unsigned char* (APICALL *FPSensorGetLastExtractImage)(int * width, int* height);

class ZkFinger
{
public:
    ZkFinger();
    ~ZkFinger();

    bool LoadSensorLibrary();
    void FreeSensorLibrary();

    // 指纹仪设备操作接口
    int InitSensor();
    int DeInitSensor();
    int GetDeviceCount();
    void *OpenDevice(int index);
    int CloseDevice(void * hDevice);
    int SetParameters(void * hDevice, int nParamCode, unsigned char* paramValue, unsigned int cbParamValue);
    int GetParameters(void * hDevice, int nParamCode, unsigned char* paramValue, unsigned int* cbParamValue);
    int AcquireFingerprint(void * hDevice, unsigned char* fpImage, unsigned int cbFPImage, unsigned char* fpTemplate, unsigned int* cbTemplate);
    int AcquireFingerprintImage(void * hDevice, unsigned char* fpImage, unsigned int cbFPImage);

    // 指纹仪算法相关接口
    bool DBInit();
    int DBFree();
    int DBMerge(unsigned char* temp1, unsigned char* temp2, unsigned char* temp3, unsigned char* regTemp, unsigned int* cbRegTemp);
    int DBAdd(unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);
    int DBDel(unsigned int fid);
    int DBClear();
    int DBCount(unsigned int* fpCount);
    int DBIdentify(unsigned char* fpTemplate, unsigned int cbTemplate, unsigned int* FID, unsigned int* score);
    int DBMatch(unsigned char* template1, unsigned int cbTemplate1, unsigned char* template2, unsigned int cbTemplate2);
    int ExtractFromImage(const char* lpFilePathName, unsigned int DPI, unsigned char* fpTemplate, unsigned int *cbTemplate);
    int Base64ToBlob(const char* src, unsigned char* blob, unsigned int cbBlob);
    int BlobToBase64(const unsigned char* src, unsigned int cbSrc, char* base64Str, unsigned int cbBase64str);
    int DBSetParameter(int nParamCode, int paramValue);
    int DBGetParameter(int nParamCode, int* paramValue);

    int VerifyByID(unsigned int fid, unsigned char* fpTemplate, unsigned int cbTemplate);
    int GetCaptureParams(void * hDevice, PZKFPCapParams pCapParams);
    int GetCaptureParamsEx(void * hDevice, int* width, int* height, int* dpi);
    unsigned char* GetLastExtractImage(int * width, int* height);

    void TestFunc();

private:
    HMODULE hLibrary;

    FPSensorInit p_sensorInit;
    FPSensorTerminate p_sensorDeInit;
    FPSensorGetDeviceCount p_sensorGetDeviceCount;
    FPSensorOpenDevice p_sensorOpenDevice;
    FPSensorCloseDevice p_sensorCloseDevice;
    FPSensorSetParameters p_sensorSetParameters;
    FPSensorGetParameters p_sensorGetParameters;
    FPSensorAcquireFingerprint p_sensorAcquireFingerprint;
    FPSensorAcquireFingerprintImage p_sensorAcquireFingerprintImage;

    FPSensorDBInit p_dbInit;
    FPSensorDBFree p_dbFree;
    FPSensorDBMerge p_dbMerge;
    FPSensorDBAdd p_dbAdd;
    FPSensorDBDel p_dbDel;
    FPSensorDBClear p_dbClear;
    FPSensorDBCount p_dbCount;
    FPSensorDBIdentify p_dbIdentify;
    FPSensorDBMatch p_dbMatch;
    FPSensorExtractFromImage p_dbExtractFromImage;
    FPSensorBase64ToBlob p_dbBase64ToBlob;
    FPSensorBlobToBase64 p_dbBlobToBase64;
    FPSensorDBSetParameter p_dbSetParameter;
    FPSensorDBGetParameter p_dbGetParameter;

    FPSensorVerifyByID p_dbVerifyByID;
    FPSensorGetCaptureParams p_sensorGetCaptureParams;

    void* m_zkfDB;
};

#endif // ZKFINGER_H
