#include "zkfinger.h"
#include "libzkfp.h"

#include <QDebug>

ZkFinger::ZkFinger()
{
    hLibrary = nullptr;

    p_sensorInit = nullptr;
    p_sensorDeInit = nullptr;
    p_sensorGetDeviceCount = nullptr;
    p_sensorOpenDevice = nullptr;
    p_sensorCloseDevice = nullptr;
    p_sensorSetParameters = nullptr;
    p_sensorGetParameters = nullptr;
    p_sensorAcquireFingerprint = nullptr;
    p_sensorAcquireFingerprintImage = nullptr;

    p_dbInit = nullptr;
    p_dbFree = nullptr;
    p_dbMerge = nullptr;
    p_dbAdd = nullptr;
    p_dbDel = nullptr;
    p_dbClear = nullptr;
    p_dbCount = nullptr;
    p_dbIdentify = nullptr;
    p_dbMatch = nullptr;
    p_dbExtractFromImage = nullptr;
    p_dbBase64ToBlob = nullptr;
    p_dbBlobToBase64 = nullptr;
    p_dbSetParameter = nullptr;
    p_dbGetParameter = nullptr;

    p_dbVerifyByID = nullptr;
    p_sensorGetCaptureParams = nullptr;

    m_zkfDB = nullptr;
}

ZkFinger::~ZkFinger()
{
    FreeSensorLibrary();
}

bool ZkFinger::LoadSensorLibrary()
{
    if (nullptr != hLibrary) {
        return true;
    }

    hLibrary = LoadLibraryA(ZKFP_LIB_NAME);
    if (nullptr == hLibrary) {
        qDebug() << "LoadLibraryA " << ZKFP_LIB_NAME << " is failed";
        return false;
    }

    p_sensorInit = (FPSensorInit)GetProcAddress(hLibrary, "ZKFPM_Init");
    p_sensorDeInit = (FPSensorTerminate)GetProcAddress(hLibrary, "ZKFPM_Terminate");
    p_sensorGetDeviceCount = (FPSensorGetDeviceCount)GetProcAddress(hLibrary, "ZKFPM_GetDeviceCount");
    p_sensorOpenDevice = (FPSensorOpenDevice)GetProcAddress(hLibrary, "ZKFPM_OpenDevice");
    p_sensorCloseDevice = (FPSensorCloseDevice)GetProcAddress(hLibrary, "ZKFPM_CloseDevice");
    p_sensorSetParameters = (FPSensorSetParameters)GetProcAddress(hLibrary, "ZKFPM_SetParameters");
    p_sensorGetParameters = (FPSensorGetParameters)GetProcAddress(hLibrary, "ZKFPM_GetParameters");
    p_sensorAcquireFingerprint = (FPSensorAcquireFingerprint)GetProcAddress(hLibrary, "ZKFPM_AcquireFingerprint");
    p_sensorAcquireFingerprintImage = (FPSensorAcquireFingerprintImage)GetProcAddress(hLibrary, "ZKFPM_AcquireFingerprintImage");

    p_dbInit = (FPSensorDBInit)GetProcAddress(hLibrary, "ZKFPM_DBInit");
    p_dbFree = (FPSensorDBFree)GetProcAddress(hLibrary, "ZKFPM_DBFree");
    p_dbMerge = (FPSensorDBMerge)GetProcAddress(hLibrary, "ZKFPM_DBMerge");
    p_dbAdd = (FPSensorDBAdd)GetProcAddress(hLibrary, "ZKFPM_DBAdd");
    p_dbDel = (FPSensorDBDel)GetProcAddress(hLibrary, "ZKFPM_DBDel");
    p_dbClear = (FPSensorDBClear)GetProcAddress(hLibrary, "ZKFPM_DBClear");
    p_dbCount = (FPSensorDBCount)GetProcAddress(hLibrary, "ZKFPM_DBCount");
    p_dbIdentify = (FPSensorDBIdentify)GetProcAddress(hLibrary, "ZKFPM_DBIdentify");
    p_dbMatch = (FPSensorDBMatch)GetProcAddress(hLibrary, "ZKFPM_DBMatch");
    p_dbExtractFromImage = (FPSensorExtractFromImage)GetProcAddress(hLibrary, "ZKFPM_ExtractFromImage");
    p_dbBase64ToBlob = (FPSensorBase64ToBlob)GetProcAddress(hLibrary, "ZKFPM_Base64ToBlob");
    p_dbBlobToBase64 = (FPSensorBlobToBase64)GetProcAddress(hLibrary, "ZKFPM_BlobToBase64");
    p_dbSetParameter = (FPSensorDBSetParameter)GetProcAddress(hLibrary, "ZKFPM_DBSetParameter");
    p_dbGetParameter = (FPSensorDBGetParameter)GetProcAddress(hLibrary, "ZKFPM_DBGetParameter");

    p_dbVerifyByID = (FPSensorVerifyByID)GetProcAddress(hLibrary, "ZKFPM_VerifyByID");
    p_sensorGetCaptureParams = (FPSensorGetCaptureParams)GetProcAddress(hLibrary, "ZKFPM_GetCaptureParams");

    if (nullptr == p_sensorInit || nullptr == p_dbInit || nullptr == p_sensorOpenDevice) {
        FreeSensorLibrary();
        qWarning() << "ZK finger sensor interface error, init failed";
        return false;
    }

    return true;
}

void ZkFinger::FreeSensorLibrary()
{
    if (nullptr != hLibrary) {
        FreeLibrary(hLibrary);
        hLibrary = nullptr;
    }

    p_sensorInit = nullptr;
    p_sensorDeInit = nullptr;
    p_sensorGetDeviceCount = nullptr;
    p_sensorOpenDevice = nullptr;
    p_sensorCloseDevice = nullptr;
    p_sensorSetParameters = nullptr;
    p_sensorGetParameters = nullptr;
    p_sensorAcquireFingerprint = nullptr;
    p_sensorAcquireFingerprintImage = nullptr;

    p_dbInit = nullptr;
    p_dbFree = nullptr;
    p_dbMerge = nullptr;
    p_dbAdd = nullptr;
    p_dbDel = nullptr;
    p_dbClear = nullptr;
    p_dbCount = nullptr;
    p_dbIdentify = nullptr;
    p_dbMatch = nullptr;
    p_dbExtractFromImage = nullptr;
    p_dbBase64ToBlob = nullptr;
    p_dbBlobToBase64 = nullptr;

    p_dbSetParameter = nullptr;
    p_dbGetParameter = nullptr;

    p_dbVerifyByID = nullptr;
    p_sensorGetCaptureParams = nullptr;
}


int ZkFinger::InitSensor()
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorInit();
}

int ZkFinger::DeInitSensor()
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorDeInit();
}

void *ZkFinger::OpenDevice(int index)
{
    if (!LoadSensorLibrary()) {
        return nullptr;
    }
    return p_sensorOpenDevice(index);
}

int ZkFinger::CloseDevice(void *hDevice)
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorCloseDevice(hDevice);
}

int ZkFinger::SetParameters(void *hDevice, int nParamCode, unsigned char *paramValue, unsigned int cbParamValue)
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorSetParameters(hDevice, nParamCode, paramValue, cbParamValue);
}

int ZkFinger::GetDeviceCount()
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorGetDeviceCount();
}

int ZkFinger::GetParameters(void *hDevice, int nParamCode, unsigned char *paramValue, unsigned int *cbParamValue)
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorGetParameters(hDevice, nParamCode, paramValue, cbParamValue);
}

int ZkFinger::AcquireFingerprint(void *hDevice, unsigned char *fpImage, unsigned int cbFPImage, unsigned char *fpTemplate, unsigned int *cbTemplate)
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorAcquireFingerprint(hDevice, fpImage, cbFPImage, fpTemplate, cbTemplate);
}

int ZkFinger::AcquireFingerprintImage(void *hDevice, unsigned char *fpImage, unsigned int cbFPImage)
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorAcquireFingerprintImage(hDevice, fpImage, cbFPImage);
}


/////////////////////////////////////////////////////////////////////////////////////////////
bool ZkFinger::DBInit()
{
    if (!LoadSensorLibrary()) {
        return false;
    }

    m_zkfDB = p_dbInit();

    if (nullptr == m_zkfDB) {
        return false;
    }

//    qInfo() << ">>>>> ZkFinger DBInit success <<<<<";
    return true;
}

int ZkFinger::DBFree()
{
    if (nullptr == m_zkfDB) {
        return 0;
    }

    return p_dbFree(m_zkfDB);
}

int ZkFinger::DBMerge(unsigned char *temp1, unsigned char *temp2, unsigned char *temp3, unsigned char *regTemp, unsigned int *cbRegTemp)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbMerge(m_zkfDB, temp1, temp2, temp3, regTemp, cbRegTemp);
}

int ZkFinger::DBAdd(unsigned int fid, unsigned char *fpTemplate, unsigned int cbTemplate)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbAdd(m_zkfDB, fid, fpTemplate, cbTemplate);
}

int ZkFinger::DBDel(unsigned int fid)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbDel(m_zkfDB, fid);
}

int ZkFinger::DBClear()
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbClear(m_zkfDB);
}

int ZkFinger::DBCount(unsigned int *fpCount)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbCount(m_zkfDB, fpCount);
}

int ZkFinger::DBIdentify(unsigned char *fpTemplate, unsigned int cbTemplate, unsigned int *FID, unsigned int *score)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbIdentify(m_zkfDB, fpTemplate, cbTemplate, FID, score);
}

int ZkFinger::DBMatch(unsigned char *template1, unsigned int cbTemplate1, unsigned char *template2, unsigned int cbTemplate2)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbMatch(m_zkfDB, template1, cbTemplate1, template2, cbTemplate2);
}

int ZkFinger::VerifyByID(unsigned int fid, unsigned char *fpTemplate, unsigned int cbTemplate)
{
    if (nullptr == m_zkfDB) {
        return -1;
    }

    return p_dbVerifyByID(m_zkfDB, fid, fpTemplate, cbTemplate);
}

int ZkFinger::GetCaptureParams(void *hDevice, PZKFPCapParams pCapParams)
{
    if (!LoadSensorLibrary()) {
        return -1;
    }

    return p_sensorGetCaptureParams(hDevice, pCapParams);
}



void ZkFinger::TestFunc()
{
    qDebug() << "----- ZkFinger Func Call Test -----";
}
