/*
 * _ZLAC8015.h
 *
 *  Created on: June 22, 2020
 *      Author: yankai
 */

#ifndef OpenKAI_src_Actuator__ZLAC8015_H_
#define OpenKAI_src_Actuator__ZLAC8015_H_

#include "../../Protocol/_Modbus.h"
#include "../_ActuatorBase.h"

namespace kai
{

class _ZLAC8015: public _ActuatorBase
{
public:
	_ZLAC8015();
	~_ZLAC8015();

	bool init(void* pKiss);
	bool start(void);
	void draw(void);
	int check(void);

protected:
	bool setMode(void);
	bool setSpeed(void);
	bool setAccel(void);
	bool setBrake(void);

	bool bComplete(void);
	bool stop(void);
	bool setSlave(int iSlave);

	bool readStatus(void);

	void updateMove(void);
	void update(void);
	static void* getUpdateThread(void* This)
	{
		((_ZLAC8015*) This)->update();
		return NULL;
	}

public:
	_Modbus* m_pMB;
	int		m_iSlave;
	int		m_iMode;
	ACTUATOR_AXIS* m_pA;

	INTERVAL_EVENT m_ieReadStatus;

};

}
#endif
