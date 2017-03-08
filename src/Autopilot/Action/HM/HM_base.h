#ifndef OPENKAI_SRC_AUTOPILOT_ACTION_HM_BASE_H_
#define OPENKAI_SRC_AUTOPILOT_ACTION_HM_BASE_H_

#include "../../../Base/common.h"
#include "../../../Protocol/_Canbus.h"
#include "../../../IO/TCP.h"
#include "../ActionBase.h"
#include "../../../Navigation/_GPS.h"


namespace kai
{

class HM_base: public ActionBase
{
public:
	HM_base();
	~HM_base();

	bool init(void* pKiss);
	bool link(void);
	void update(void);
	void updateCAN(void);
	bool draw(void);

	void cmd(void);
	void updateGPS(void);

public:
	_Canbus* m_pCAN;
	_GPS*	m_pGPS;
	IO* m_pCMD;
	string m_strCMD;

	int m_maxRpmT;
	int m_maxRpmW;
	int m_rpmL;
	int m_rpmR;
	int m_defaultRpmT;
	int m_motorRpmW;
	bool m_bSpeaker;

	uint8_t m_ctrlB0;
	uint8_t m_ctrlB1;

	double	m_wheelR;
	uint64_t m_lastUpdateGPS;

};

}

#endif

