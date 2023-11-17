/*
 * _LivoxAutoScan.cpp
 *
 *  Created on: May 28, 2020
 *      Author: yankai
 */

#include "_LivoxAutoScan.h"

namespace kai
{
	_LivoxAutoScan::_LivoxAutoScan()
	{
		m_nP = 0;
		m_nPmax = INT_MAX;
		m_rVoxel = 0.1;
		m_fProcess.clearAll();
		m_baseDir = "";
		m_dir = "";
		m_rB = 0.0;
		
		m_tWait = 5;
		m_pAct = NULL;
	}

	_LivoxAutoScan::~_LivoxAutoScan()
	{
	}

	bool _LivoxAutoScan::init(void *pKiss)
	{
		IF_F(!this->_PCstream::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		pK->v("rVoxel", &m_rVoxel);
		pK->v("nPmax", &m_nPmax);
		pK->v("baseDir", &m_baseDir);
		pK->v("tWait", &m_tWait);

		pK->v("vRangeH", &m_actH.m_vRange);
		pK->v("nDivH", &m_actH.m_nD);
		pK->v("vRangeV", &m_actV.m_vRange);
		pK->v("nDivV", &m_actV.m_nD);

		m_fProcess.set(lvScanner_reset);

		return true;
	}

	bool _LivoxAutoScan::link(void)
	{
		IF_F(!this->_PCstream::link());

		Kiss *pK = (Kiss *)m_pKiss;

		string n;
		n = "";
		F_ERROR_F(pK->v("_ActuatorBaseH", &n));
		m_actH.m_pAct = (_ActuatorBase *)(pK->getInst(n));
		NULL_Fl(m_actH.m_pAct, n + ": not found");

		n = "";
		F_ERROR_F(pK->v("_ActuatorBaseV", &n));
		m_actV.m_pAct = (_ActuatorBase *)(pK->getInst(n));
		NULL_Fl(m_actV.m_pAct, n + ": not found");

		vector<string> vGB;
		pK->a("vGeometryBase", &vGB);
		for (string p : vGB)
		{
			_GeometryBase *pGB = (_GeometryBase *)(pK->getInst(p));
			IF_CONT(!pGB);

			m_vpGB.push_back(pGB);
		}

		return true;
	}

	bool _LivoxAutoScan::start(void)
	{
		NULL_F(m_pT);
		IF_F(!m_pT->start(getUpdate, this));

		return true;
	}

	int _LivoxAutoScan::check(void)
	{
		IF__(m_vpGB.empty(), -1);
		NULL__(m_pAct, -1);

		return this->_PCstream::check();
	}

	void _LivoxAutoScan::update(void)
	{
		while (m_pT->bRun())
		{
			m_pT->autoFPSfrom();

			if (m_fProcess.b(lvScanner_reset, true))
				scanReset();

			if (m_fProcess.b(lvScanner_start, true))
				scanStart();

			if (m_fProcess.b(lvScanner_stop, true))
				scanStop();

			if (m_fProcess.b(lvScanner_save, true))
				savePC();

			scanUpdate();

			m_pT->autoFPSto();
		}
	}

	void _LivoxAutoScan::scanReset(void)
	{
		IF_(check() < 0);

		_Livox *pPsrc = (_Livox *)m_vpGB[0];
		pPsrc->clear();

		m_nP = 0;
		for (int i = 0; i < m_vPC.size(); i++)
		{
			PointCloud *pP = &m_vPC[i];
			pP->Clear();
		}
		m_vPC.clear();
	}

	void _LivoxAutoScan::scanStart(void)
	{
		IF_(check() < 0);

		m_actH.updateConfig();
		m_actV.updateConfig();
		m_actH.reset();
		m_actV.reset();

		// Actuator reset pos
		// todo


		m_bScanning = true;
		_Livox *pPsrc = (_Livox *)m_vpGB[0];
		pPsrc->startStream();
	}

	void _LivoxAutoScan::scanStop(void)
	{
		IF_(check() < 0);

		_Livox *pPsrc = (_Livox *)m_vpGB[0];
		pPsrc->stopStream();

	 	m_bScanning = false;
	}

	void _LivoxAutoScan::scanUpdate(void)
	{
		IF_(check() < 0);
		IF_(!m_bScanning);

		m_actV.update();
		if(m_actV.bComplete())
		{
			m_actV.reset();
			m_actH.update();
		}

		if(m_actH.bComplete())
		{
			scanStop();
			return;
		}

		m_actH.m_pAct->setPtarget(0, m_actH.m_v, true);
		m_actV.m_pAct->setPtarget(0, m_actV.m_v, true);
		sleep(m_tWait);

		_Livox *pPsrc = (_Livox *)m_vpGB[0];
//		pPsrc->setTranslation();
		pPsrc->clear();	
		sleep(m_tWait);		//TODO: change to point number

		PointCloud pc;
		pPsrc->getPC(&pc);
		int nPnew = pc.points_.size();
		IF_(nPnew <= 0);

		// Add original
		m_vPC.push_back(pc);
		m_nP += nPnew;
		m_rB = (float)m_nP / (float)m_nPmax;
	}

	void _LivoxAutoScan::savePC(void)
	{
		IF_(check() < 0);
		IF_(m_nP <= 0);

		// Make new folder
		m_dir = m_baseDir + tFormat() + "/";
		string cmd = "mkdir " + m_dir;
		system(cmd.c_str());

		io::WritePointCloudOption par;
		par.write_ascii = io::WritePointCloudOption::IsAscii::Binary;
		par.compressed = io::WritePointCloudOption::Compressed::Uncompressed;

		PointCloud pcMerge;
		int nSave = 0;
		for (int i = 0; i < m_vPC.size(); i++)
		{
			PointCloud *pP = &m_vPC[i];
			nSave += (io::WritePointCloudToPLY(m_dir + i2str(i) + ".ply",
											   *pP,
											   par)) ? 1 : 0;

			pcMerge += *pP;
		}

		nSave += (io::WritePointCloudToPLY(m_dir + "_merged.ply",
										   pcMerge,
										   par)) ? 1 : 0;
	}

	void _LivoxAutoScan::resetScan(void)
	{
		m_fProcess.set(lvScanner_reset);
	}

	void _LivoxAutoScan::startScan(void)
	{
		m_fProcess.set(lvScanner_start);
	}

	void _LivoxAutoScan::stopScan(void)
	{
		m_fProcess.set(lvScanner_stop);
	}

	void _LivoxAutoScan::save(void)
	{
		m_fProcess.set(lvScanner_save);
	}

	float _LivoxAutoScan::getBufferCap(void)
	{
		return m_rB;
	}

}
