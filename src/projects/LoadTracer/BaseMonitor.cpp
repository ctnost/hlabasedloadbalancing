#include "StdAfx.h"
#include "BaseMonitor.h"
using namespace LoadMonitor;

BaseMonitor::BaseMonitor(void)
{
	dao = gcnew LoadTracer();
	dao->ConnectToDb();
	tracer = new ResourceTracer();
}


LOAD_TYPES BaseMonitor::GetType()
{
	return this->loadType;
}

double BaseMonitor::GetUtilization()
{
	return this->utilization;
}