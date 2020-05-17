#pragma once
#include "BaseMonitor.h"
using namespace System;

namespace LoadMonitor
{
	ref class MemoryMonitor : public BaseMonitor
	{
	public:
		MemoryMonitor(LOAD_TYPES lType);
		virtual void Start() override;
		virtual void Monitor() override;
		
	public:
		~MemoryMonitor(void);
	};
}
