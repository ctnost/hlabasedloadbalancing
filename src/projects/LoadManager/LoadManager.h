// LoadManager.h

#pragma once

#include "../LoadTracer/Defines.h"
#include "LoadViewer.h"
#include "RemoteLoadManager.h"

using namespace System;
using namespace System::Threading;
using namespace System::Collections::Generic;
using namespace LoadMonitor;
using namespace LoadHistory;

namespace LoadManager 
{
	//Load Monitor durdurup baþlatabilcek
	//Konfigürasyonlarý vericek
	//LoadMonitor den aldýðý bilgiler doðrultusunda ortalama yük izlenip karar verilecek
	public ref class LoadController
	{
	public:
		LoadController(LoadViewer^ loadv);

		void PauseMonitoring();
		void StartMonitoring();
		void StopMonitoring();

		void TraceLoad();

		LoadTracer^ GetDAO(){ return dao;}

	private:
		LoadMonitorManager^ loadMM;
		LoadTracer^ dao;
		LoadViewer^ lViewer;
		RemoteLoadManager^ rlm;
		List<String^> ^activeNodeList;

		LOAD_STATES GetLoadState(double val, double highThreshold, double lowThreshold);
		LOAD_STATES GetPCState(LOAD_STATES cpuState,LOAD_STATES cpuqState,LOAD_STATES memState,LOAD_STATES bwState, double cpu_u,double cpu_q,double mem_u,double bw_u);
		double AvgList(List<double>^ list);
		void InitLoadViewer();
		void DrawUIByType(LOAD_TYPES lType,double usage,double avg,double highThreshold, double lowThreshold,LOAD_STATES state);
		System::Drawing::Graphics^ loadUICpuLoadGraphic;
		System::Drawing::Graphics^ loadUICpuQueueLoadGraphic;
		System::Drawing::Graphics^ loadUIMemoryLoadGraphic;
		System::Drawing::Graphics^ loadUINetworkLoadGraphic;
	};
}
