#include "StdAfx.h"
#include "NetworkMonitor.h"

using namespace LoadMonitor;

NetworkMonitor::NetworkMonitor(LOAD_TYPES lType)
{
	loadType = lType;
}

NetworkMonitor::~NetworkMonitor(void)
{
}

void NetworkMonitor::Monitor()
{
	int net_q = tracer->GetBW_U()*8;
	double net_s = tracer->GetBW_S();
	double net_l = (net_q / net_s)*100;
	dao->SetNETLoad(net_l,ipAddress);

	/*double maxNetLoad = dao->GetNETMaxLoad();
	double net_w = 0.1;
	if (net_l > maxNetLoad)
	{
		net_w = maxNetLoad;
	}
	if(maxNetLoad != 0)
		net_w = (net_l / maxNetLoad) * 100;
	dao->SetNETWeight(net_w,ipAddress);*/
	this->utilization = net_l;
}

void NetworkMonitor::Start()
{
	while (true)
	{
		Monitor();
	}
}