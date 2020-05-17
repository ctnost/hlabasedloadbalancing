#include "StdAfx.h"
#include "ProcessorMonitor.h"

using namespace LoadMonitor;

ProcessorMonitor::ProcessorMonitor(LOAD_TYPES lType)
{
	loadType = lType;
}

ProcessorMonitor::~ProcessorMonitor(void)
{
}

void ProcessorMonitor::Monitor()
{
	/*
	int cpu_q = tracer->GetCPU_Q();
	double cpu_s = tracer->GetCPU_S();
	double cpu_l = cpu_q / cpu_s;
	dao->SetCPULoad(cpu_l,ipAddress);

	double maxCpuLoad = dao->GetCPUMaxLoad();
	double cpu_w = 0.1;
	if (cpu_l > maxCpuLoad)
	{
		cpu_w = maxCpuLoad;
	}
	if(maxCpuLoad != 0)
		cpu_w = (cpu_l / maxCpuLoad) * 100;
	*/
	double cpu_u = (int)tracer->GetCPU_Usage(); 
	dao->SetCPULoad(cpu_u,ipAddress);
	this->utilization = cpu_u;
}

void ProcessorMonitor::Start()
{
	while (true)
	{
		Monitor();
	}
}
