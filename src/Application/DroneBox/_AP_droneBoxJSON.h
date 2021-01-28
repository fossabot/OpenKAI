#ifndef OpenKAI_src_Application_DroneBox__AP_droneBoxJSON_H_
#define OpenKAI_src_Application_DroneBox__AP_droneBoxJSON_H_

#include "../../Protocol/_JSONbase.h"
#include "../../Application/Autopilot/ArduPilot/_AP_gs.h"

namespace kai
{

class _AP_droneBoxJSON : public _JSONbase
{
public:
	_AP_droneBoxJSON();
	~_AP_droneBoxJSON();

	virtual bool init(void* pKiss);
	virtual bool start(void);
	virtual int check(void);
	virtual void draw(void);
    
protected:
	void send(void);
    
    //msg handlers
	void handleMsg(string& str);
    void heartbeat(picojson::object& o);
    void ackLandingRequest (picojson::object& o);
    void ackTakeoffRequest (picojson::object& o);

private:
	void updateW(void);
	static void* getUpdateThreadW(void* This)
	{
		((_AP_droneBoxJSON*) This)->updateW();
		return NULL;
	}

	void updateR(void);
	static void* getUpdateThreadR(void* This)
	{
		((_AP_droneBoxJSON*) This)->updateR();
		return NULL;
	}

public:
    _AP_gs* m_pAPgs;
};

}
#endif