#pragma once
#include "BaseMonitor.h"

namespace LoadMonitor
{
	ref class NetworkMonitor : public BaseMonitor
	{
	public:
		NetworkMonitor(LOAD_TYPES lType);
		virtual void Start() override;
		virtual void Monitor() override;
		
	public:
		~NetworkMonitor(void);
	};
}
