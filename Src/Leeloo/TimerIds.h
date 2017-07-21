#ifndef LEELOO_TIMER_IDS_H
#define LEELOO_TIMER_IDS_H

#include <string>

namespace TimerIds
{
	enum Id
	{
		TimerIds_PRIMARY_RAY_STAGE,
		TimerIds_INTERSECTION_STAGE,
		TimerIds_COLOR_STAGE,
		TimerIds_CNT
	};

	static const std::string NAMES[] = {
		"PrimaryRayStage", "IntersectionStage", "ColorStage", "Count"};	
};

#endif //LEELOO_TIMER_IDS_H