#pragma once
#include "../LoadTracer/Defines.h"
using namespace System;
using namespace System::Collections::Generic;

ref class RemoteNodeServer
{
public:
	RemoteNodeServer(void);
	void StartRemoteNodeServer();
	int ComputeLoadPointForMigration();
	LOAD_STATES nodeState;
	List<String^>^ nodeList;
	String^ utilizations;
	List<String^>^ GetActiveFedList();
	String^ ipAddress;
};

