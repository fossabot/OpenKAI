/*
 * _InRange.cpp
 *
 *  Created on: April 23, 2019
 *      Author: yankai
 */

#include "_InRange.h"

namespace kai
{

	_InRange::_InRange()
	{
		m_type = vision_inRange;
		m_pV = NULL;
		m_vRange.set(0.0, FLT_MAX);
	}

	_InRange::~_InRange()
	{
		close();
	}

	bool _InRange::init(void *pKiss)
	{
		IF_F(!_VisionBase::init(pKiss));
		Kiss *pK = (Kiss *)pKiss;

		pK->v("vRange", &m_vRange);

		return true;
	}

	bool _InRange::link(void)
	{
		IF_F(!this->_VisionBase::link());

		Kiss *pK = (Kiss *)m_pKiss;

		string n;
		n = "";
		pK->v("_VisionBase", &n);
		m_pV = (_VisionBase *)(pK->getInst(n));
		IF_Fl(!m_pV, n + ": not found");

		return true;
	}

	bool _InRange::open(void)
	{
		NULL_F(m_pV);
		m_bOpen = m_pV->isOpened();

		return m_bOpen;
	}

	void _InRange::close(void)
	{
		this->_VisionBase::close();
	}

	bool _InRange::start(void)
	{
		NULL_F(m_pT);
		return m_pT->start(getUpdate, this);
	}

	void _InRange::update(void)
	{
		while (m_pT->bRun())
		{
			if (!m_bOpen)
				open();

			m_pT->autoFPSfrom();

			if (m_bOpen)
			{
				filter();
			}

			m_pT->autoFPSto();
		}
	}

	void _InRange::filter(void)
	{
		IF_(m_pV->getFrameRGB()->bEmpty());

		Mat m;
		cv::inRange(*m_pV->getFrameRGB()->m(), m_vRange.x, m_vRange.y, m);
		m_fRGB.copy(m);
	}

}
