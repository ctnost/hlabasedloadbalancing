#include "StdAfx.h"
#include "CPUQueueMonitor.h"

using namespace LoadMonitor;

CPUQueueMonitor::CPUQueueMonitor(LOAD_TYPES lType)
{
	loadType = lType;
}

CPUQueueMonitor::~CPUQueueMonitor(void)
{
}

void CPUQueueMonitor::Monitor()
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
	double cpu_q = (int)tracer->GetCPU_Q(); 
	dao->SetCPUQueue(cpu_q,ipAddress);
	this->utilization = cpu_q;
}

void CPUQueueMonitor::Start()
{
	while (true)
	{
		Monitor();
	}
}
