/*
 * Window.cpp
 *
 *  Created on: July 10, 2023
 *      Author: Kai Yan
 */

#include "_WebUIswarm.h"

namespace kai
{

	_WebUIswarm::_WebUIswarm()
	{
	}

	_WebUIswarm::~_WebUIswarm()
	{
	}

	bool _WebUIswarm::init(void *pKiss)
	{
		IF_F(!this->_WebUIbase::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		//		pK->v("rootDir", &m_rootDir);

		return true;
	}

	bool _WebUIswarm::link(void)
	{
		IF_F(!this->_WebUIbase::link());
		Kiss *pK = (Kiss *)m_pKiss;

		string n;
		n = "";
		pK->v("_StateControl", &n);
		m_pSC = (_StateControl *)(pK->getInst(n));

		vector<string> vB;
		pK->a("vAPbase", &vB);
		for (string p : vB)
		{
			_AP_base *pB = (_AP_base *)(pK->getInst(p));
			IF_CONT(!pB);

			m_vpAP.push_back(pB);
		}

		return true;
	}

	bool _WebUIswarm::start(void)
	{
		NULL_F(m_pT);
		NULL_F(m_pTui);

		m_pT->start(getUpdate, this);
		return m_pTui->start(getShow, this);
	}

	int _WebUIswarm::check(void)
	{
		NULL__(m_pSC, -1);

		return this->_WebUIbase::check();
	}

	void _WebUIswarm::update(void)
	{
		while (m_pT->bRun())
		{
			m_pT->autoFPSfrom();

			updateVehicles();

			m_pT->autoFPSto();
		}
	}

	void _WebUIswarm::updateVehicles(void)
	{
		string h = "";

		for(int i=0; i<m_vpAP.size(); i++)
		{
			_AP_base* pAP = m_vpAP[i];
			
			h += "<tr>";
            h += "<th scope=\"row\">"+ i2str(i) +"</th>";
            h += "<td>"+ pAP->getApModeName() +"</td>";

			vFloat3 vAtt = pAP->getApAttitude();
            h += "<td>";
			h += f2str(vAtt.x) + ", ";
			h += f2str(vAtt.y) + ", ";
			h += f2str(vAtt.z);
			h += "</td>";

			float hdg = pAP->getApHdg();
            h += "<td>"+ f2str(hdg) +"</td>";

			vDouble4 vGpos = pAP->getGlobalPos();
            h += "<td>"+ f2str(vGpos.w) +"</td>";

			float batt = pAP->getBattery();
            h += "<td>"+ f2str(batt) +"</td>";

			h += "</tr>";
		}

		string s = "setHTML('tbVehicles', '" + h + "')";
		webui_run(m_wd, s.c_str());

		s = "setHTML('state', '"+ m_pSC->getStateName() + "')";
		webui_run(m_wd, s.c_str());
	}

	void _WebUIswarm::show(void)
	{
		m_wd = webui_new_window();
		g_cbWebUIswarm[g_nWebUIswarm].m_wd = m_wd;
		g_cbWebUIswarm[g_nWebUIswarm].m_pI = this;
		g_nWebUIswarm++;

		webui_set_root_folder(m_wd, m_rootDir.c_str());
		webui_bind(m_wd, "btnStandby", cbBtn);
		webui_bind(m_wd, "btnTakeoff", cbBtn);
		webui_bind(m_wd, "btnAuto", cbBtn);
		webui_bind(m_wd, "btnRTL", cbBtn);
		webui_show(m_wd, m_fHtml.c_str());
		webui_wait();
		m_wd = 0;

		exit(0);
	}

	void _WebUIswarm::btn(webui_event_t *e)
	{
		IF_(check()<0);

		string eID = string(e->element);
		if (eID == "btnStandby")
		{
			m_pSC->transit("STANDBY");
		}
		else if (eID == "btnTakeoff")
		{
			m_pSC->transit("TAKEOFF");
		}
		else if (eID == "btnAuto")
		{
			m_pSC->transit("AUTO");
		}
		else if (eID == "btnRTL")
		{
			m_pSC->transit("RTL");
		}
	}

}