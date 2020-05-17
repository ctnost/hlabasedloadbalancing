// ProgramRunner.cpp : main project file.

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <sstream>  

using namespace std;
using std::cout;
using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;
using namespace System::Net;
using namespace System::Net::NetworkInformation;

using namespace LoadManager;
using namespace LoadMonitor;

void ShowActiveTcpConnections()
{
	Console::WriteLine( "Active TCP Connections" );
	IPGlobalProperties ^ properties = IPGlobalProperties::GetIPGlobalProperties();
	array<TcpConnectionInformation^>^connections = properties->GetActiveTcpConnections();
	System::Collections::IEnumerator^ myEnum6 = connections->GetEnumerator();
	while ( myEnum6->MoveNext() )
	{
		TcpConnectionInformation ^ c = safe_cast<TcpConnectionInformation ^>(myEnum6->Current);
		
		Console::WriteLine( "{0} <==> {1}", c->LocalEndPoint, c->RemoteEndPoint );
	}
}



int main(array<System::String ^> ^args)
{
	//SHowActivePcList();
	//ShowActiveTcpConnections();
	LoadViewer^ loadv = gcnew LoadViewer();
	LoadController^ lController = gcnew LoadController(loadv);
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( lController, &LoadController::StartMonitoring );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();
	//ToDo:D
	loadv->ShowDialog();
/*
	Process::Start( "SimAgent.exe", "WSN");
	cout << " >>>>>>>>>> Please Enter Wireless Sensor Number <<<<<<<<<<" << endl;
	string line;
	getline( cin, line );
	int wsCount = atoi(line.c_str());
	for (int i = 0; i < wsCount; i++)
	{
		Process::Start( "SimAgent.exe", "WS");
		Thread::Sleep(5000);
	}
*/
    return 0;
}
