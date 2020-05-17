#pragma once
#include "BaseMonitor.h"

namespace LoadMonitor
{
	ref class CPUQueueMonitor : public BaseMonitor
	{
	public:
		CPUQueueMonitor(LOAD_TYPES lType);
		virtual void Start() override;
		virtual void Monitor() override;

	public:
		~CPUQueueMonitor(void);
	};
}
