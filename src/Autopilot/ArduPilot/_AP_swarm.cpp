#include "_AP_swarm.h"

namespace kai
{

	_AP_swarm::_AP_swarm()
	{
		m_pAP = NULL;
		m_pXb = NULL;
		m_pSwarm = NULL;
		m_pGio = NULL;
		m_pU = NULL;

		m_bAutoArm = true;
		m_altTakeoff = 10.0;
		m_altAuto = 10.0;
		m_altLand = 8.0;
		m_myID = 0;

		m_vTargetID.clear();
		m_iClass = 1;
		m_dRdet = 1e-6;

		m_ieNextWP.init(USEC_10SEC);
		m_vWP.set(0, 0);
		m_vWPd.set(5, 5);
		m_vWPrange.set(10, 10);

		m_ieSendHB.init(USEC_1SEC);
		m_ieSendGCupdate.init(USEC_1SEC);
	}

	_AP_swarm::~_AP_swarm()
	{
	}

	bool _AP_swarm::init(void *pKiss)
	{
		IF_F(!this->_AP_move::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		pK->v("bAutoArm", &m_bAutoArm);
		pK->v("altTakeoff", &m_altTakeoff);
		pK->v("altAuto", &m_altAuto);
		pK->v("altLand", &m_altLand);
		pK->v("myID", &m_myID);

		pK->v("vTargetID", &m_vTargetID);
		pK->v("iClass", &m_iClass);
		pK->v("dRdet", &m_dRdet);

		pK->v("vWPd", &m_vWPd);
		pK->v("vWPrange", &m_vWPrange);
		pK->v("ieNextWP", &m_ieNextWP.m_tInterval);

		pK->v("ieSendHB", &m_ieSendHB.m_tInterval);
		pK->v("ieSendGCupdate", &m_ieSendGCupdate.m_tInterval);

		return true;
	}

	bool _AP_swarm::link(void)
	{
		IF_F(!this->_AP_move::link());
		IF_F(!m_pSC);
		m_state.STANDBY = m_pSC->getStateIdxByName("STANDBY");
		m_state.TAKEOFF = m_pSC->getStateIdxByName("TAKEOFF");
		m_state.AUTO = m_pSC->getStateIdxByName("AUTO");
		m_state.RTL = m_pSC->getStateIdxByName("RTL");
		IF_F(!m_state.bValid());

		Kiss *pK = (Kiss *)m_pKiss;
		string n;

		n = "";
		pK->v("_AP_base", &n);
		m_pAP = (_AP_base *)(pK->getInst(n));
		IF_Fl(!m_pAP, n + ": not found");

		n = "";
		pK->v("_SwarmSearch", &n);
		m_pSwarm = (_SwarmSearch *)(pK->getInst(n));
		IF_Fl(!m_pSwarm, n + ": not found");

		n = "";
		pK->v("_Xbee", &n);
		m_pXb = (_Xbee *)(pK->getInst(n));
		IF_Fl(!m_pXb, n + ": not found");

		n = "";
		pK->v("_IObase", &n);
		m_pGio = (_IObase *)(pK->getInst(n));
		IF_Fl(!m_pGio, n + ": not found");

		n = "";
		pK->v("_Universe", &n);
		m_pU = (_Universe *)(pK->getInst(n));
		IF_Fl(!m_pU, n + ": not found");

		IF_F(!m_pXb->setCbReceivePacket(sOnRecvMsg, this));

		return true;
	}

	bool _AP_swarm::start(void)
	{
		NULL_F(m_pT);
		return m_pT->start(getUpdate, this);
	}

	int _AP_swarm::check(void)
	{
		NULL__(m_pAP, -1);
		NULL__(m_pAP->m_pMav, -1);
		NULL__(m_pSC, -1);
		NULL__(m_pXb, -1);
		NULL__(m_pSwarm, -1);
		NULL__(m_pU, -1);

		return this->_AP_move::check();
	}

	void _AP_swarm::update(void)
	{
		while (m_pT->bRun())
		{
			m_pT->autoFPSfrom();
			this->_StateBase::update();

			updateState();

			send();

			m_pT->autoFPSto();
		}
	}

	void _AP_swarm::updateState(void)
	{
		IF_(check() < 0);

		int apMode = m_pAP->getApMode();
		bool bApArmed = m_pAP->bApArmed();
		float alt = m_pAP->getGlobalPos().w; // relative altitude

		m_state.update(m_pSC->getStateIdx());

		// Standby
		if (m_state.bSTANDBY())
		{
			stop();
			return;
		}

		// Takeoff
		if (m_state.bTAKEOFF())
		{
			if (apMode != AP_COPTER_GUIDED)
			{
				m_pAP->setApMode(AP_COPTER_GUIDED);
				return;
			}

			if (!bApArmed)
			{
				if (m_bAutoArm)
					m_pAP->setApArm(true);

				return;
			}

			if (alt > m_altTakeoff)
			{
				m_pSC->transit(m_state.AUTO);
				return;
			}

			// add some extra to make it easier for takeoff completion detection
			m_pAP->m_pMav->clNavTakeoff(m_altTakeoff + 1.0);
			return;
		}

		// Auto
		if (m_state.bAUTO())
		{
			IF_(apMode != AP_COPTER_GUIDED);

			IF_(findBeacon());
			if (!findVisual())
			{
				calcMove();
				return;
			}

			stop();

			return;
		}

		// RTL
		if (m_state.bRTL())
		{
			if (apMode == AP_COPTER_GUIDED)
				m_pAP->setApMode(AP_COPTER_RTL);
		}
	}

	bool _AP_swarm::findBeacon(void)
	{
		IF_F(check() < 0);

		SWARM_NODE *pN = m_pSwarm->getNodeByIDrange(m_vTargetID);
		NULL_F(pN);
		IF_F(!pN->m_bPosValid);

		vDouble4 vP;
		vP.x = pN->m_vPos.x;
		vP.y = pN->m_vPos.y;
		vP.z = m_altAuto;
		vP.w = 0;
		setPglobal(vP, false, false);

		// SWARM_DETECTION *pD = getDetByDist(pN->m_vPos, m_dRdet);
		// IF_T(pD);

		// SWARM_DETECTION d;
		// d.m_id = pN->m_id;
		// d.m_vPos = pN->m_vPos;
		// m_vDetections.push_back(d);

		return true;
	}

	bool _AP_swarm::findVisual(void)
	{
		IF_F(check() < 0);

		gimbalDownward();

		IF_F(!findVisualTarget());

		vDouble4 vPos = m_pAP->getGlobalPos();
		vDouble2 vP;
		vP.x = vPos.x;
		vP.y = vPos.y;
		SWARM_DETECTION *pD = getDetByDist(vP, m_dRdet);
		IF_T(pD);

		pD = getDetByID(m_myID + 200);
		if (pD)
		{
			pD->m_vPos = vP;
			return true;
		}

		SWARM_DETECTION d;
		d.m_id = m_myID + 200;
		d.m_vPos = vP;
		m_vDetections.push_back(d);

		return true;
	}

	bool _AP_swarm::findVisualTarget(void)
	{
		IF_F(check() < 0);

		_Object *pO;
		int i = 0;
		while ((pO = m_pU->get(i++)) != NULL)
		{
			IF_CONT(pO->getTopClass() != m_iClass);

			return true;
		}

		return false;
	}

	void _AP_swarm::calcMove(void)
	{
		uint64_t t = getApproxTbootUs();
		IF_(!m_ieSendHB.update(t));

		m_vWP += m_vWPd;

		vDouble4 vWP;
		vWP.x = m_vWP.x;
		vWP.y = m_vWP.y;
		vWP.z = 0;
		vWP.w = 0;
		setPlocal(vWP);

		if (m_vWP.x >= m_vWPrange.x)
			m_vWPd.x = -abs(m_vWPd.x);
		else if (m_vWP.x <= -m_vWPrange.x)
			m_vWPd.x = abs(m_vWPd.x);

		if (m_vWP.y >= m_vWPrange.y)
			m_vWPd.y = -abs(m_vWPd.y);
		else if (m_vWP.y <= -m_vWPrange.y)
			m_vWPd.y = abs(m_vWPd.y);
	}

	SWARM_DETECTION *_AP_swarm::getDetByID(uint16_t id)
	{
		for (int i = 0; i < m_vDetections.size(); i++)
		{
			SWARM_DETECTION *pD = &m_vDetections[i];
			IF_CONT(pD->m_id != id);

			return pD;
		}

		return NULL;
	}

	SWARM_DETECTION *_AP_swarm::getDetByDist(vDouble2 vP, double r)
	{
		for (int i = 0; i < m_vDetections.size(); i++)
		{
			SWARM_DETECTION *pD = &m_vDetections[i];

			vDouble2 vD = pD->m_vPos - vP;
			IF_CONT(vD.len() > r);

			return pD;
		}

		return NULL;
	}

	void _AP_swarm::gimbalDownward(void)
	{
		NULL_(m_pGio);
		IF_(!m_pGio->bOpen());

		string cmd = "#TPUG2wPTZ02";
		char crc = calcCRC(cmd.c_str(), (int)cmd.length());

		char buf[3];
		snprintf(buf, 3, "%X", crc);
		cmd += string(buf);

		m_pGio->write((uint8_t *)cmd.c_str(), (int)cmd.length());
	}

	char _AP_swarm::calcCRC(const char *cmd, uint8_t len)
	{
		char crc = 0;
		for (int i = 0; i < len; i++)
		{
			crc += cmd[i];
		}
		return crc;
	}

	void _AP_swarm::send(void)
	{
		IF_(check() < 0);

		uint64_t t = m_pT->getTfrom();

		if (m_ieSendHB.update(t))
		{
			sendHB();
			sendDetectionHB();
		}

		// if (m_ieSendGCupdate.update(t))
		//     sendGCupdate();
	}

	void _AP_swarm::sendHB(void)
	{
		IF_(check() < 0);

		vDouble4 vP = m_pAP->getGlobalPos();
		SWMSG_HB m;
		m.m_srcID = m_myID;
		m.m_lat = vP.x * 1e7;
		m.m_lng = vP.y * 1e7;
		m.m_alt = vP.w * 1e2;
		m.m_hdg = m_pAP->getApHdg() * 1e1;
		m.m_spd = m_pAP->getApSpeed().len() * 1e2;
		m.m_batt = m_pAP->getBattery() * 1e2;
		m.m_mode = m_pSC->getStateIdx();

		uint8_t pB[XB_N_PAYLOAD];
		int nB = m.encode(pB, XB_N_PAYLOAD);
		IF_(nB <= 0);

		m_pXb->send(XB_BRDCAST_ADDR, pB, nB);
	}

	void _AP_swarm::sendDetectionHB(void)
	{
		IF_(check() < 0);

		for (int i = 0; i < m_vDetections.size(); i++)
		{
			SWARM_DETECTION *pD = &m_vDetections[i];

			SWMSG_HB m;
			m.m_srcID = pD->m_id;
			m.m_lat = pD->m_vPos.x * 1e7;
			m.m_lng = pD->m_vPos.y * 1e7;
			uint8_t pB[XB_N_PAYLOAD];
			int nB = m.encode(pB, XB_N_PAYLOAD);
			IF_CONT(nB <= 0);

			m_pXb->send(XB_BRDCAST_ADDR, pB, nB);
		}
	}

	void _AP_swarm::sendGCupdate(void)
	{
		SWMSG_GC_UPDATE m;
		m.m_srcID = m_myID;
		m.m_dstID = 0;
		m.m_iGC = 0;
		m.m_w = 1;

		uint8_t pB[XB_N_PAYLOAD];
		int nB = m.encode(pB, XB_N_PAYLOAD);
		IF_(nB <= 0);

		m_pXb->send(XB_BRDCAST_ADDR, pB, nB);
	}

	void _AP_swarm::onRecvMsg(const XBframe_receivePacket &d)
	{
		uint8_t mType = d.m_pD[0];

		switch (mType)
		{
		case swMsg_hB:
		{
			SWMSG_HB mHb;
			mHb.m_srcNetAddr = d.m_srcAddr;
			if (mHb.decode(d.m_pD, d.m_nD))
				m_pSwarm->handleMsgHB(mHb);
		}
		break;
		case swMsg_cmd_setState:
		{
			SWMSG_CMD_SETSTATE mSS;
			mSS.m_srcNetAddr = d.m_srcAddr;
			if (mSS.decode(d.m_pD, d.m_nD))
				handleMsgSetState(mSS);
		}
		break;
		case swMsg_gc_update:
		{
			SWMSG_GC_UPDATE mGU;
			mGU.m_srcNetAddr = d.m_srcAddr;
			if (mGU.decode(d.m_pD, d.m_nD))
				m_pSwarm->handleMsgGCupdate(mGU);
		}
		break;
		}
	}

	void _AP_swarm::handleMsgSetState(const SWMSG_CMD_SETSTATE &m)
	{
		IF_(check() < 0);
		IF_((m.m_dstID != XB_BRDCAST_ADDR) && (m.m_dstID != m_pXb->getMyAddr()));

		m_state.update(m_pSC->getStateIdx());

		// TODO: enable iMsg counter

		switch (m.m_state)
		{
		case swMsg_state_standBy:
			m_pSC->transit(m_state.STANDBY);
			m_vWP.set(0, 0);
			break;
		case swMsg_state_takeOff:
			if (m_state.bSTANDBY())
			{
				m_pSC->transit(m_state.TAKEOFF);
			}
			break;
		case swMsg_state_auto:
			if (!m_state.bRTL())
			{
				m_pSC->transit(m_state.AUTO);
			}
			break;
		case swMsg_state_RTL:
			m_pSC->transit(m_state.RTL);
			break;
		case swMsg_state_land:
			m_pSC->transit(m_state.RTL);
			break;
		}
	}

}
