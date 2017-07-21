#include "Timer.h"
#include "Utility.h"

#include <iostream>
#include <fstream>

Timer::Timer()
{
	m_start		= nullptr;
	m_stop		= nullptr;
	m_disjoint	= nullptr;
}

Timer::~Timer()
{
	SAFE_RELEASE(m_start);
	SAFE_RELEASE(m_stop);
	SAFE_RELEASE(m_disjoint);
}

void Timer::start(ID3D11DeviceContext* p_context)
{
	p_context->Begin(m_disjoint);
	p_context->End(m_start);
}

void Timer::stop(ID3D11DeviceContext* p_context)
{
	p_context->End(m_stop);
	p_context->End(m_disjoint);
}

double Timer::getTime(ID3D11DeviceContext* p_context)
{
	UINT64 startTime = 0;
	while(p_context->GetData(m_start, &startTime, sizeof(startTime), 0) != S_OK);

	UINT64 endTime = 0;
	while(p_context->GetData(m_stop, &endTime, sizeof(endTime), 0) != S_OK);

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
	while(p_context->GetData(m_disjoint, &disjointData, sizeof(disjointData), 0) != S_OK);

	double time = -1.0;
	if(disjointData.Disjoint == FALSE)
	{
		UINT64 delta = endTime - startTime;
		double frequency = static_cast<double>(disjointData.Frequency);
		time = (delta/frequency) * 1000.0;
		m_tempTimes.push_back(time);
	}
	return time;
}

void Timer::finnishFrame()
{
	double total = 0.0;
	for(unsigned int i=0; i<m_tempTimes.size(); i++)
		total += m_tempTimes[i];
	m_tempTimes.clear();
	m_times.push_back(total);
}

unsigned int Timer::getNumSamples()
{
	return m_times.size();
}

HRESULT Timer::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_QUERY_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Query		= D3D11_QUERY_TIMESTAMP_DISJOINT;
	desc.MiscFlags	= 0;
	
	hr = p_device->CreateQuery(&desc, &m_disjoint);

	desc.Query = D3D11_QUERY_TIMESTAMP;

	if(SUCCEEDED(hr))
		hr = p_device->CreateQuery(&desc, &m_start);
	if(SUCCEEDED(hr))
		hr = p_device->CreateQuery(&desc, &m_stop);

	return hr;
}

void Timer::print()
{
	for(unsigned int i=0; i<m_times.size(); i++)
		std::cout << m_times[i] << std::endl;
}

void Timer::printToFile(std::string p_filename)
{
	double min, max, avg;
	calcMinMaxAvg(&min, &max, &avg);
	double stdDev = calcStandardDeviation(avg);

	std::ofstream outFile(p_filename.c_str());
	outFile << p_filename << "\n\n";

	outFile << "Min: " << min << "\n";
	outFile << "Max: " << max << "\n\n";

	outFile << "Avg: " << avg << "\n";
	outFile << "StdDev: " << stdDev << "\n\n";

	outFile << "DataCnt " << m_times.size() << "\n";

	outFile << "Data" << "\n";
	for(unsigned int i=0; i<m_times.size(); i++)
		outFile << m_times[i] << "\n";
}

double Timer::calcStandardDeviation(double p_avg) 
{
	double stdDev = 0.0;
	for(unsigned int i=0; i<m_times.size(); i++)
		stdDev += (m_times[i]-p_avg)*(m_times[i]-p_avg);
	return sqrt(stdDev/m_times.size());
}

void Timer::calcMinMaxAvg(double* io_min, double* io_max, double* io_avg)
{
	double min = DBL_MAX;
	double max = 0.0;
	double avg = 0.0;
	for(unsigned int i=0; i<m_times.size(); i++)
	{
		avg += m_times[i];
		if(m_times[i] < min)
			min = m_times[i];
		if(m_times[i] > max)
			max = m_times[i];
	}
	(*io_min) = min;
	(*io_max) = max;
	(*io_avg) = avg/m_times.size();
}
