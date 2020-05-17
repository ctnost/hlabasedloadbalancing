// This is the main DLL file.

#include "stdafx.h"

#include "ResourceTracer.h"
#include "Defines.h"

using namespace LoadMonitor;
using namespace System;
using namespace System::Threading;

ResourceTracer::ResourceTracer()
{
}

ResourceTracer::~ResourceTracer()
{

}

int ResourceTracer::GetCPU_Q()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PerfRawData_PerfOS_System"); 
	int totalCpuQueue = 0;
	for (int i = 0; i < LOAD_NUMBER; i++)
	{
		for each (ManagementObject^ queryObj in searcher->Get())
		{
			totalCpuQueue += Convert::ToInt32(queryObj["ProcessorQueueLength"]);
		}
		//Wait
		Thread::Sleep(LOAD_PERIOD - 100);
	}

	totalCpuQueue = totalCpuQueue / LOAD_NUMBER;

	return totalCpuQueue;
}

int ResourceTracer::GetCPU_Usage()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PerfFormattedData_PerfOS_Processor"); 
	int totalCpuUsage = 0;
	for (int i = 0; i < LOAD_NUMBER; i++)
	{
		for each (ManagementObject^ queryObj in searcher->Get())
		{
			totalCpuUsage += Convert::ToInt32(queryObj["PercentProcessorTime"]);
		}
		//Wait
		Thread::Sleep(LOAD_PERIOD - 100);
	}

	totalCpuUsage = totalCpuUsage / (LOAD_NUMBER*2);

	return totalCpuUsage;
}

double ResourceTracer::GetCPU_S()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_Processor"); 
	int totalCpu = 0;

	for each (ManagementObject^ queryObj in searcher->Get())
	{
		totalCpu += Convert::ToInt32(queryObj["MaxClockSpeed"]);
	}

	return totalCpu;
}

double ResourceTracer::GetMEM_U()
{
	ManagementObjectSearcher ^searcher
	  = gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_OperatingSystem"); 
	double totalMemUsage = 0.0;
	for (int i = 0; i < LOAD_NUMBER; i++)
	{
		for each (ManagementObject^ queryObj in searcher->Get())
		{
			 totalMemUsage += Convert::ToDouble(queryObj["FreePhysicalMemory"]);
		}
		//Wait
		Thread::Sleep(LOAD_PERIOD - 100);
	}
	
	totalMemUsage = totalMemUsage / LOAD_NUMBER;

	return totalMemUsage;
}

double ResourceTracer::GetMEM_S()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_OperatingSystem"); 
	double totalMem = 0.0;
	for each (ManagementObject^ queryObj in searcher->Get())
	{
		totalMem += Convert::ToDouble(queryObj["TotalVisibleMemorySize"]);
	}
	return totalMem;
}

double ResourceTracer::GetMEM_Usage()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PerfFormattedData_PerfOS_Memory"); 
	double totalMemUsage = 0.0;
	for (int i = 0; i < LOAD_NUMBER; i++)
	{
		for each (ManagementObject^ queryObj in searcher->Get())
		{
			totalMemUsage += Convert::ToDouble(queryObj["PercentCommittedBytesInUse"]);
		}
		//Wait
		Thread::Sleep(LOAD_PERIOD - 100);
	}

	totalMemUsage = totalMemUsage / LOAD_NUMBER;
	return totalMemUsage;
}

double ResourceTracer::GetBW_U()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PerfFormattedData_Tcpip_NetworkInterface"); 
	double totalMemUsage = 0.0;
	for (int i = 0; i < LOAD_NUMBER; i++)
	{
		for each (ManagementObject^ queryObj in searcher->Get())
		{
			totalMemUsage += Convert::ToDouble(queryObj["BytesTotalPersec"]);
			break;
		}
		//Wait
		Thread::Sleep(LOAD_PERIOD - 100);
	}

	totalMemUsage = totalMemUsage / LOAD_NUMBER;

	return totalMemUsage;
}

double ResourceTracer::GetBW_S()
{
	ManagementObjectSearcher ^searcher
		= gcnew ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PerfFormattedData_Tcpip_NetworkInterface"); 
	double totalNet = 0.0;
	for each (ManagementObject^ queryObj in searcher->Get())
	{
		totalNet += Convert::ToDouble(queryObj["CurrentBandwidth"]);
		break;
	}
	return totalNet;
}