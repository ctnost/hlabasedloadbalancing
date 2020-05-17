#include "StdAfx.h"
#include "RemoteLoadManager.h"
#include <list>
#include "StringOperation.h"
#include "..\Federates\CountryRTI\Defaults.h"
#include <ctime>
using namespace std;

using namespace System;
using namespace System::Text;
using namespace System::Net::Sockets;
using namespace System::Threading;

RemoteLoadManager::RemoteLoadManager(LoadTracer^ dao_)
{
	rnc = gcnew RemoteNodeClient();
	rns = gcnew RemoteNodeServer();
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( rns, &RemoteNodeServer::StartRemoteNodeServer );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();
	dao = dao_;
}

struct MigrationStatus
{
	int migrationPoint;
	string ip;
};

struct SortMigrationStatus : public std::binary_function<MigrationStatus*, MigrationStatus*, bool>
{
	bool operator()(MigrationStatus* left, MigrationStatus* right) const
	{
		return(left->migrationPoint < right->migrationPoint);
	};
};

List<String^>^ RemoteLoadManager::GetRemoteUtilizations()
{
	List<String^>^ rUtilizations = gcnew List<String^>();
	for each(String^ node in nodeList){
		String^ utils = rnc->GetUtilization(node);
		if(utils!="")
			rUtilizations->Add(utils+" "+node);
	}
	return rUtilizations;
}

List<String^>^ RemoteLoadManager::GetRemoteFederates(String^ ipAddress)
{
	List<String^>^ rFeds = gcnew List<String^>();
	String^ feds = rnc->GetFederates(ipAddress);
	cli::array<String^>^ fedarray = feds->Split(' ');
	for (int i = 0; i < fedarray->Length; i++)
	{
		rFeds->Add(fedarray[i]);
	}
	
	return rFeds;
}

void RemoteLoadManager::ManageLoads(String^ ipAddress, List<LoadViewer::PCNodeStruct^>^ lowLoadedNodeList,double cpuHighThreshold,double memHighThreshold)
{
	migStartTime = time(0);
	dao->UpdateMigrationTable(migStartTime);
	
	list<MigrationStatus*> migStatus;	
	for each(LoadViewer::PCNodeStruct^ pcListItem in lowLoadedNodeList)
	{
		String^ node = pcListItem->ipAddress;
		int nodePoint = rnc->AskForMigration(node);
		Console::WriteLine("IPAddress:" + node + "NodePoint" + nodePoint);
		MigrationStatus* migs = new MigrationStatus();
		migs->ip = StringOperation::toStandardString(node);
		migs->migrationPoint = nodePoint;
		migStatus.push_back(migs);
	}
	migStatus.sort(SortMigrationStatus());
	int selectionCount = 0;
	bool migrationHasStarted = false;
	for(list<MigrationStatus*>::iterator migrationit = migStatus.begin();migrationit!=migStatus.end();migrationit++)
	{
		MigrationStatus* migStatus = static_cast<MigrationStatus*>(*migrationit);
		if(migStatus->migrationPoint == -5 || migStatus->migrationPoint > 9000)
			continue;
		String^ targetIp = StringOperation::toSystemString(migStatus->ip);
		int remoteMST = rnc->HasAnyMigrationRequest(targetIp);
		Console::WriteLine("remoteMST:" + remoteMST + "-migStartTime:"+migStartTime);
		if(remoteMST == 0 /*|| remoteMST > migStartTime*/)
		{
			String^ migratingFed = ChooseFederate(ipAddress);
			if (migratingFed == ""){
				break;
			}
			if(!CheckNodeFutureLoad(targetIp,lowLoadedNodeList,migratingFed,ipAddress,cpuHighThreshold,memHighThreshold))
				continue;
			Console::WriteLine("Tasima Basliyor : " + migratingFed + " -> " + targetIp);
			migratingTargetIp = targetIp;
			migratingFederate = migratingFed;
			rnc->SetMigrationStartTime(targetIp,migStartTime);
			MigrateFederate(ipAddress,targetIp,migratingFed);
			migrationHasStarted = true;
			break;
		}
	}
	if(!migrationHasStarted){
		dao->UpdateMigrationTable(0);
		migStartTime = 0;
	}
}

bool RemoteLoadManager::CheckNodeFutureLoad(String^ targetIp,List<LoadViewer::PCNodeStruct^>^ lowLoadedNodeList, String^ fedName,String^ ipAddress,double cpuHighThreshold,double memHighThreshold)
{
	List<double>^ fedUsages = dao->GetFederateResourceUsages(fedName,ipAddress);
	double fedCpuU = fedUsages[0];
	double fedMemU = fedUsages[1];
	for each(LoadViewer::PCNodeStruct^ pcListItem in lowLoadedNodeList){
		if(targetIp == pcListItem->ipAddress){
			double newCPUUsage =pcListItem->cpu_u+fedCpuU;
			double newMEMUsage =pcListItem->mem_u+fedMemU;
			Console::WriteLine("*****" + pcListItem->cpu_u.ToString() + "--" + newCPUUsage.ToString());
			Console::WriteLine("*****" + pcListItem->mem_u.ToString() + "--" + newMEMUsage.ToString());
			if(newCPUUsage > cpuHighThreshold && newMEMUsage > memHighThreshold || newMEMUsage > 90){
				Console::WriteLine("**IT IS WORK NOT MIGRATE THIS*** :" + targetIp);
				return false;
			}
			else 
				return true;
		}
	}
}

String^ RemoteLoadManager::ChooseFederate(String^ ipAddress)
{
	LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
	dao->ConnectToDb();
	List<String^>^ activeFedList = rns->GetActiveFedList();
	if(activeFedList->Count == 0)
		return "";
	List<String^>^ migratableFederates = dao->GetMigratableFederates(ipAddress, activeFedList);
	if(migratableFederates->Count == 0)
		return "";
	double maxFedLife = 0;
	String^ bestFedName = "";
	for each(String^ fedName in migratableFederates){
		double currFedLife = dao->GetFederateLife(fedName,ipAddress);
		if(currFedLife > maxFedLife){
			maxFedLife = currFedLife;
			bestFedName = fedName;
		}
	}
	String^ nullFed = "";
	//int maxPow = MaxPower;
	if(maxFedLife > 0 && maxFedLife <= 15)
		return nullFed;
	return bestFedName;
}

void RemoteLoadManager::MigrateFederate(String^ myIp,String^ targetIp, String^ fedName)
{
	TcpClient^ clientSocket = gcnew TcpClient();
	try
	{
		clientSocket->Connect(myIp,8887);
		NetworkStream^ networkStream = clientSocket->GetStream();
		String^ message = targetIp +"|"+fedName;
		array<Byte>^ data = Encoding::Unicode->GetBytes(message);
		networkStream->Write(data, 0, data->Length);
		networkStream->Flush();
	}
	catch (Exception^ ex)
	{
		ex->ToString();
	}

	clientSocket->Close();
}
