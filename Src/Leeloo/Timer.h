#ifndef LEELOO_TIMER_H
#define LEELOO_TIMER_H

#include <vector>
#include <string>
#include <iostream>
#include <d3d11.h>

class Timer
{
public:
	Timer();
	~Timer();

	void start(ID3D11DeviceContext* p_context);
	void stop(ID3D11DeviceContext* p_context);

	double getTime(ID3D11DeviceContext* p_context);
	void finnishFrame();
	unsigned int getNumSamples();

	HRESULT init(ID3D11Device* p_device);

	void print();
	void printToFile(std::string p_filename);

private:
	double calcStandardDeviation(double p_avg);
	void calcMinMaxAvg(double* io_min, double* io_max, double* io_avg);

	ID3D11Query* m_start;
	ID3D11Query* m_stop;
	ID3D11Query* m_disjoint;

	std::vector<double> m_tempTimes;
	std::vector<double> m_times;
};

#endif