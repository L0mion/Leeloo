#include "ManagementTimer.h"
#include "Timer.h"
#include "Utility.h"

ManagementTimer::ManagementTimer(std::string p_filename)
{
	m_filename = p_filename;

	for(unsigned int i=0; i<TimerIds::TimerIds_CNT; i++)
		m_timers[i] = nullptr;
}

ManagementTimer::~ManagementTimer()
{
	for(unsigned int i=0; i<TimerIds::TimerIds_CNT; i++)
	{
		SAFE_DELETE(m_timers[i]);
	}
}

HRESULT ManagementTimer::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	for(unsigned int i=0; i<TimerIds::TimerIds_CNT; i++)
	{
		if(SUCCEEDED(hr))
		{
			m_timers[i] = new Timer();
			hr = m_timers[i]->init(p_device);
		}
	}
	return hr;
}

void ManagementTimer::start(TimerIds::Id p_timerId, ID3D11DeviceContext* p_context)
{
	m_timers[p_timerId]->start(p_context);
}

void ManagementTimer::stop(TimerIds::Id p_timerId, ID3D11DeviceContext* p_context)
{
	m_timers[p_timerId]->stop(p_context);
	m_timers[p_timerId]->getTime(p_context);
}

void ManagementTimer::finnishFrame()
{
	for(unsigned int i=0; i<TimerIds::TimerIds_CNT; i++)
		m_timers[i]->finnishFrame();
}

void ManagementTimer::printToFile()
{
	for(unsigned int i=0; i<TimerIds::TimerIds_CNT; i++)
	{
		m_timers[i]->printToFile("Results/" + TimerIds::NAMES[i] + m_filename);
	}
}

void ManagementTimer::printToFile(TimerIds::Id p_timerId)
{
	m_timers[p_timerId]->printToFile(m_filename);
}