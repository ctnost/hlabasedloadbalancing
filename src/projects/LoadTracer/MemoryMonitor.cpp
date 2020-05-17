#include "StdAfx.h"
#include "MemoryMonitor.h"

using namespace LoadMonitor;

MemoryMonitor::MemoryMonitor(LOAD_TYPES lType)
{
	loadType = lType;
}

MemoryMonitor::~MemoryMonitor(void)
{
}

void MemoryMonitor::Monitor()
{
/*
	double mem_s = tracer->GetMEM_S();
	double mem_u = tracer->GetMEM_U();
	double mem_l = (mem_s-mem_u) / mem_s;
	dao->SetMEMLoad(mem_l,ipAddress);

	double maxMemLoad = dao->GetMEMMaxLoad();
	double mem_w = 0.1;
	if (mem_l > maxMemLoad)
	{
		mem_w = maxMemLoad;
	}
	if(maxMemLoad != 0)
		mem_w = (mem_l / maxMemLoad) * 100;
	dao->SetMEMWeight(mem_w,ipAddress);
*/
	double mem_u = (int)tracer->GetMEM_Usage(); 
	dao->SetMEMLoad(mem_u,ipAddress);
	this->utilization = mem_u;
}

void MemoryMonitor::Start()
{
	while (true)
	{
		Monitor();
	}
}
