/*
 *  Created on: Jan 19, 2024
 *      Author: yankai
 */
#include "LivoxLidar2.h"

namespace kai
{

    LivoxLidar2::LivoxLidar2()
    {
        m_fConfig = "";
        m_bOpen = false;
    }

    LivoxLidar2::~LivoxLidar2()
    {
    }

    bool LivoxLidar2::init(void *pKiss)
    {
        IF_F(!this->BASE::init(pKiss));
        Kiss *pK = (Kiss *)pKiss;

        pK->v("fConfig", &m_fConfig);
        //        pK->v("lidarMode", (int*)&m_lidarMode);

        return true;
    }

    bool LivoxLidar2::open(void)
    {
        if (!LivoxLidarSdkInit(m_fConfig.c_str()))
        {
            LOG_I("Livox2 Init Failed");
            LivoxLidarSdkUninit();
            return false;
        }

        SetLivoxLidarPointCloudCallBack(sCbPointCloud, this);
        SetLivoxLidarImuDataCallback(sCbImuData, this);
        //        SetLivoxLidarInfoCallback(LivoxLidarPushMsgCallback, this);
        SetLivoxLidarInfoChangeCallback(sCbLidarInfoChange, this);

        m_bOpen = true;

        LOG_I("open() success");
        return true;
    }

    void LivoxLidar2::close(void)
    {
        LivoxLidarSdkUninit();
    }

    void LivoxLidar2::startStream(void)
    {
        //        m_lidarMode = kLivoxLidarNormal;
    }

    void LivoxLidar2::stopStream(void)
    {
        //        m_lidarMode = kLivoxLidarSleep;
    }

    int LivoxLidar2::getDeviceHandle(const string &SN)
    {
        LivoxLidar2device *pD = getDevice(SN);
        if(pD)
            return pD->m_handle;

        return -1;
    }

    LivoxLidar2device *LivoxLidar2::getDevice(const string &SN)
    {
        for (int i = 0; i < m_vDevice.size(); i++)
        {
            LivoxLidar2device *pD = &m_vDevice[i];
            IF_CONT(pD->m_SN != SN);

            return pD;
        }

        return NULL;
    }

    LivoxLidar2device *LivoxLidar2::getDevice(uint32_t handle)
    {
        for (int i = 0; i < m_vDevice.size(); i++)
        {
            LivoxLidar2device *pD = &m_vDevice[i];
            IF_CONT(pD->m_handle != handle);

            return pD;
        }

        return NULL;
    }

    bool LivoxLidar2::setCbData(uint32_t handle, CbLivoxLidar2data pCb, void *pLivox2)
    {
        NULL_F(pLivox2);
        NULL_F(pCb);

        LivoxLidar2device *pD = getDevice(handle);
        NULL_F(pD);

        pD->m_pCbData = pCb;
        pD->m_pLivox2 = pLivox2;
        return true;
    }

    bool LivoxLidar2::setLidarMode(uint32_t handle, LivoxLidarWorkMode m)
    {
        LivoxLidar2device *pD = getDevice(handle);
        NULL_F(pD);

        // TODO
    }

    bool LivoxLidar2::setScanPattern(uint32_t handle, LivoxLidarScanPattern p)
    {
        LivoxLidar2device *pD = getDevice(handle);
        NULL_F(pD);

        // TODO
    }

    void LivoxLidar2::CbPointCloud(uint32_t handle, const uint8_t dev_type, LivoxLidarEthernetPacket *pD)
    {
        NULL_(pD);
        LOG_I("CbPointCloud handle: " + i2str(handle) + ", data_num: " + i2str(pD->dot_num) + ", data_type: " + i2str(pD->data_type) + ", length: " + i2str(pD->length) + ", frame_counter: " + i2str(pD->frame_cnt));

        LivoxLidar2device *pDev = getDevice(handle);
        NULL_(pDev);
        pDev->m_pCbData(pD, pDev->m_pLivox2);
    }

    void LivoxLidar2::CbImuData(uint32_t handle, const uint8_t dev_type, LivoxLidarEthernetPacket *pD)
    {
        NULL_(pD);
        LOG_I("CbImuData, handle:" + i2str(handle) + ", data_num:" + i2str(pD->dot_num) + ", data_type:" + i2str(pD->data_type) + ", length:" + i2str(pD->length) + ", frame_counter:" + i2str(pD->frame_cnt));

        LivoxLidar2device *pDev = getDevice(handle);
        NULL_(pDev);
        pDev->m_pCbIMU(pD, pDev->m_pLivox2);
    }

    void LivoxLidar2::CbWorkMode(livox_status status, uint32_t handle, LivoxLidarAsyncControlResponse *pR)
    {
        NULL_(pR);
        LOG_I("CbWorkMode, status:" + i2str(status) + ", handle:" + i2str(handle) + ", ret_code:" + i2str(pR->ret_code) + ", error_key:" + i2str(pR->error_key));
    }

    void LivoxLidar2::CbLoggerStart(livox_status status, uint32_t handle, LivoxLidarLoggerResponse *pR)
    {
        NULL_(pR);

        if (status != kLivoxLidarStatusSuccess)
        {
            LOG_I("Start logger failed, status: " + i2str(status));
            LivoxLidarStartLogger(handle, kLivoxLidarRealTimeLog, sCbLoggerStart, this);
            return;
        }

        if (!pR)
        {
            LOG_I("Start logger failed, response NULL");
            LivoxLidarStartLogger(handle, kLivoxLidarRealTimeLog, sCbLoggerStart, this);
            return;
        }

        if (pR->ret_code != 0)
        {
            LOG_I("Start logger failed, ret_Code: " + i2str(pR->ret_code));
            LivoxLidarStartLogger(handle, kLivoxLidarRealTimeLog, sCbLoggerStart, this);
            return;
        }

        LOG_I("Start logger succ. " + i2str(handle));
    }

    void LivoxLidar2::CbDebugPointCloud(livox_status status, uint32_t handle, LivoxLidarLoggerResponse *pR)
    {
        NULL_(pR);

        LOG_I("livox_status = " + i2str(status) + ", Lidar: " + i2str(handle) + " response: " + i2str(pR->ret_code));
    }

    void LivoxLidar2::CbLidarInfoChange(const uint32_t handle, const LivoxLidarInfo *pI)
    {
        NULL_(pI);

        LOG_I("LidarInfoChangeCallback Lidar handle: " + i2str(handle) + " SN: " + string(pI->sn));
        SetLivoxLidarWorkMode(handle, kLivoxLidarNormal, sCbWorkMode, this);
        // LivoxLidarStartLogger(handle, kLivoxLidarRealTimeLog, sCbLoggerStart, this);
        //  SetLivoxLidarDebugPointCloud(handle, true, sCbDebugPointCloud, this);
        //  sleep(10);
        //  SetLivoxLidarDebugPointCloud(handle, false, DebugPointCloudCallback, nullptr);
    }

    void LivoxLidar2::console(void *pConsole)
    {
        NULL_(pConsole);
        this->BASE::console(pConsole);

        _Console *pC = (_Console *)pConsole;
        //		pC->addMsg("nState: " + i2str(m_vStates.size()), 0);
    }

}
