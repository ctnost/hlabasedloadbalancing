#include "StdAfx.h"
#include "LoadMonitorManager.h"
#include "MemoryMonitor.h"
#include "ProcessorMonitor.h"
#include "CPUQueueMonitor.h"
#include "NetworkMonitor.h"
using namespace System::Threading;
using namespace LoadMonitor;
using namespace System::Net;

LoadMonitorManager::LoadMonitorManager(void)
{
	monitorList = gcnew List<BaseMonitor^>();
}

String^ LoadMonitorManager::GetIP()
{
	String^ strHostName = "";
	strHostName = System::Net::Dns::GetHostName();
	IPHostEntry^ ipEntry = System::Net::Dns::GetHostEntry(strHostName);
	cli::array<IPAddress^>^ addr = ipEntry->AddressList;
	return addr[addr.Length-1]->ToString();
}
void LoadMonitorManager::CreateLoadTracerList()
{
	//Daha sonra konfigürasyon dosyasýndan okunarak seçileecek.
	//BaseMonitor^ memMonitor = gcnew MemoryMonitor();
	//monitorList->Add(memMonitor);
}

void LoadMonitorManager::StartMonitoring()
{
	String^ ip = GetIP();
	MemoryMonitor^ memMonitor = gcnew MemoryMonitor(LOAD_TYPES::MEM);
	memMonitor->ipAddress = ip; 
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( memMonitor, &MemoryMonitor::Start );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();
	monitorList->Add(memMonitor);

	ProcessorMonitor^ cpuMonitor = gcnew ProcessorMonitor(LOAD_TYPES::CPU);
	cpuMonitor->ipAddress = ip;
	ThreadStart^ threadDelegate2 = gcnew ThreadStart( cpuMonitor, &ProcessorMonitor::Start );
	Thread^ newThread2 = gcnew Thread( threadDelegate2 );
	newThread2->Start();
	monitorList->Add(cpuMonitor);

	NetworkMonitor^ netMonitor = gcnew NetworkMonitor(LOAD_TYPES::BW);
	netMonitor->ipAddress = ip;
	ThreadStart^ threadDelegate3 = gcnew ThreadStart( netMonitor, &NetworkMonitor::Start );
	Thread^ newThread3 = gcnew Thread( threadDelegate3 );
	newThread3->Start();
	monitorList->Add(netMonitor);

	CPUQueueMonitor^ cpuqMonitor = gcnew CPUQueueMonitor(LOAD_TYPES::CPUQ);
	cpuqMonitor->ipAddress = ip;
	ThreadStart^ threadDelegate4 = gcnew ThreadStart( cpuqMonitor, &CPUQueueMonitor::Start );
	Thread^ newThread4 = gcnew Thread( threadDelegate4 );
	newThread4->Start();
	monitorList->Add(cpuqMonitor);
}

double LoadMonitorManager::GetUtilization(LOAD_TYPES lType)
{
	for each(BaseMonitor^ monitor in monitorList)
	{
		if(monitor->GetType() == lType)
			return monitor->GetUtilization();
	}

	return -1;
}

double LoadMonitorManager::GetLocalUtilization(LOAD_TYPES lType)
{
	for each(BaseMonitor^ monitor in monitorList)
	{
		if(monitor->GetType() == lType)
			return monitor->localUtilization;
	}

	return -1;
}