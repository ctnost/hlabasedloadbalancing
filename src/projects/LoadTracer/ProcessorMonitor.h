#pragma once
#include "BaseMonitor.h"

namespace LoadMonitor
{
	ref class ProcessorMonitor : public BaseMonitor
	{
	public:
		ProcessorMonitor(LOAD_TYPES lType);
		virtual void Start() override;
		virtual void Monitor() override;

	public:
		~ProcessorMonitor(void);
	};
}
