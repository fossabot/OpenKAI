#ifndef OpenKAI_src_Autopilot_AP__AP_video_H_
#define OpenKAI_src_Autopilot_AP__AP_video_H_

#include "../../Vision/_VisionBase.h"
#include "_AP_base.h"

namespace kai
{

	class _AP_video : public _StateBase
	{
	public:
		_AP_video();
		~_AP_video();

		bool init(void *pKiss);
		bool start(void);
		void update(void);
		int check(void);
		void console(void *pConsole);

	private:
		bool openStream(void);
		void closeStream(void);
		void writeStream(void);
		static void *getUpdate(void *This)
		{
			((_AP_video *)This)->update();
			return NULL;
		}

	private:
		_AP_base *m_pAP;
		_VisionBase *m_pV;

		string m_gstOutput;
		VideoWriter m_gst;
		vInt2 m_vSize;

		string m_dir;
		string m_subDir;
		bool m_bFlipRGB;
		bool m_bFlipD;
	};

}
#endif
