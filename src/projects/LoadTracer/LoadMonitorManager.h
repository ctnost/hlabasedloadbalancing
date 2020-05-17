#pragma once
using namespace System::Collections::Generic;
using namespace System;

#include "BaseMonitor.h"

namespace LoadMonitor
{
	/// <summary>
	/// Yük izleyicisi yönetici sýnýfý.
	/// </summary>
	public ref class LoadMonitorManager
	{
	public:
		LoadMonitorManager(void);
		void StartMonitoring();
		double GetUtilization(LOAD_TYPES lType);
		double GetLocalUtilization(LOAD_TYPES lType);
		String^ GetIP();

	private:
		List<BaseMonitor^>^ monitorList;

	private:
		void CreateLoadTracerList();
	};
}