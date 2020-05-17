#pragma once

#include "Defines.h"
#include "ResourceTracer.h"

using namespace System::Threading;
using namespace System;
using namespace LoadHistory;

namespace LoadMonitor
{
	ref class BaseMonitor
	{
	public:
		BaseMonitor(void);
	public:
		virtual void Start()=0;
		virtual void Monitor()=0;

		LOAD_TYPES GetType();
		double GetUtilization();
	public:
		String^ ipAddress;
		LoadTracer^ dao;
		IResourceTracer* tracer;
		LOAD_TYPES loadType;
		double utilization;
		double localUtilization;
	};
}