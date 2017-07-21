#ifndef LEELOO_MANAGEMENT_TIMER_H
#define LEELOO_MANAGEMENT_TIMER_H

#include <d3d11.h>
#include <string>
#include "TimerIds.h"

class Timer;

class ManagementTimer
{
public:

	ManagementTimer(std::string p_filename);
	~ManagementTimer();

	HRESULT init(ID3D11Device* p_device);

	void start(TimerIds::Id p_timerId, ID3D11DeviceContext* p_context);
	void stop(TimerIds::Id p_timerId, ID3D11DeviceContext* p_context);

	void finnishFrame();

	void printToFile();
	void printToFile(TimerIds::Id p_timerId);
private:	
	std::string m_filename;

	Timer* m_timers[TimerIds::TimerIds_CNT];
};

#endif //LEELOO_MANAGEMENT_TIMER_H