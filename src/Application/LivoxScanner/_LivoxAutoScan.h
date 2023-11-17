/*
 * _LivoxAutoScan.h
 *
 *  Created on: May 28, 2020
 *      Author: yankai
 */

#ifndef OpenKAI_src_Application_LidarScanner__LivoxAutoScan_H_
#define OpenKAI_src_Application_LidarScanner__LivoxAutoScan_H_

#include "../../3D/PointCloud/_PCstream.h"
#include "../../LIDAR/Livox/_Livox.h"
#include "../../Utility/BitFlag.h"
#include "../../Actuator/_ActuatorBase.h"

namespace kai
{
	struct LivoxAutoScanActuator
	{
		// config
		vFloat2 m_vRange;
		float m_dV = 0.01;
		int m_nD;
		_ActuatorBase* m_pAct = NULL;
		// dynamic
		float m_v = 0.5;

		void updateConfig(void)
		{
			m_dV = m_vRange.d() / m_nD;
		}

		void reset(void)
		{
			m_v = m_vRange.x;
		}

		bool bComplete(void)
		{
			IF_T(m_v >= m_vRange.y);

			return false;
		}

		float update(void)
		{
			if(m_v < m_vRange.y)
				m_v += m_dV;

			return m_v;
		}
	};

    enum LidarAutoScan_threadBit
	{
		lvScanner_reset,
		lvScanner_start,
		lvScanner_stop,
		lvScanner_save,
	};

	class _LivoxAutoScan : public _PCstream
	{
	public:
		_LivoxAutoScan();
		virtual ~_LivoxAutoScan();

		virtual bool init(void *pKiss);
		virtual bool link(void);
		virtual bool start(void);
		virtual int check(void);

		virtual void resetScan(void);
		virtual void startScan(void);
		virtual void stopScan(void);
		virtual void save(void);

		float getBufferCap(void);

	protected:
		//point cloud
		virtual void savePC(void);
		virtual void scanUpdate(void);
		virtual void scanStop(void);
		virtual void scanStart(void);
		virtual void scanReset(void);
		virtual void update(void);
		static void *getUpdate(void *This)
		{
			((_LivoxAutoScan *)This)->update();
			return NULL;
		}

	protected:
		vector<_GeometryBase *> m_vpGB;

		vector<PointCloud> m_vPC;
		int m_nP;
		int m_nPmax;
		float m_rVoxel;
		string m_baseDir;
		string m_dir;
		int m_tWait;

		LivoxCtrl m_livoxCtrl;
		BIT_FLAG m_fProcess;
		float m_rB;

		bool m_bScanning;
		int m_nTake;
		_ActuatorBase* m_pAct;
		LivoxAutoScanActuator m_actH;
		LivoxAutoScanActuator m_actV;

	};

}
#endif
