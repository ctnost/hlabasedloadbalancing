#include "StdAfx.h"
#include "RemoteNodeServer.h"
#include "Utility.h"

using namespace System::IO;
using namespace System::IO::Pipes;
using namespace System::Net::Sockets;

RemoteNodeServer::RemoteNodeServer(void)
{
}

void RemoteNodeServer::StartRemoteNodeServer()
{
	TcpListener^ fedListener = gcnew TcpListener(8881); 
	fedListener->Start(); 

	while(true)
	{
		Socket^ socket = fedListener->AcceptSocket(); 
		NetworkStream^ nStream = gcnew NetworkStream(socket);

		String^ request = Utility::ReadString(nStream);
		if(Utility::CheckFedState(request,"AvailableForMigration"))
		{
			int mpoint = ComputeLoadPointForMigration();
			Utility::WriteString(mpoint.ToString(),nStream);
		}
		else if(Utility::CheckFedState(request,"MigrationRequest"))
		{
			LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
			dao->ConnectToDb();
			long migTime = dao->GetMigrationTime();
			// Daha önce migrationa baþlamýþ ise baslama zamani
			Utility::WriteString(migTime.ToString(),nStream);
		}
		else if(Utility::CheckFedState(request,"GetUtilization")){
			if(utilizations)
				Utility::WriteString(utilizations,nStream);
			else
				Utility::WriteString("0 0 0 0",nStream);
		}
		else if(Utility::CheckFedState(request,"GetFederates")){
			List<String^>^  activeFedList = GetActiveFedList();
			String^ afList = "";
			for each(String^ fed in activeFedList){
				afList += (fed + " ");
			}
			if(afList != "")
				afList = afList->Substring(0,afList->Length-1);
			Utility::WriteString(afList,nStream);
		}
		else if(request->Contains("StartTime")){
			LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
			dao->ConnectToDb();
			cli::array<String^>^ ar = request->Split('|');
			long migrationStartTime = Convert::ToInt64(ar[1]);
			dao->UpdateMigrationTable(migrationStartTime);
		}
		nStream->Flush() ; 
		socket->Close();
	}
}
List<String^>^ RemoteNodeServer::GetActiveFedList()
{
	List<String^>^ activeFedList = gcnew List<String^>();
	cli::array<String^> ^ pipeList = System::IO::Directory::GetFiles("\\\\.\\pipe\\");
	for (int i = 0; i < pipeList->Length; i++){
		String^ pipe = pipeList[i];
		if(pipe->Contains("WS")){
			int indexx = pipe->IndexOf("WS");
			String^ fedNamex = pipe->Substring(indexx);
			activeFedList->Add(fedNamex);
		}
	}
	return activeFedList;
}

int RemoteNodeServer::ComputeLoadPointForMigration()
{
	int totalPoint = 0;
	switch (this->nodeState)
	{
	case LOAD_STATES::ST_IDLE:
		totalPoint = 0;
		break;
	case LOAD_STATES::ST_LOW:
		totalPoint = 100;
		break;
	case LOAD_STATES::ST_HIGH:
		totalPoint = 10000;
		break;
	default:
		totalPoint = 1000;
	}
	
	// Federenin kosum suresi kalan bitme zamani
	List<String^>^ federares = GetActiveFedList();
	LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
	dao->ConnectToDb();
	for each(String^ fedName in federares)
	{
		totalPoint += dao->GetFederateLife(fedName,ipAddress);
	}

	//TODO : yeni basliyacak federe icin bir yuk puan eklenecek.
	// Federe aktarim suresi ve aktarim boyutu
	// Dugumlerin yuk yogunlugu onceki kosumlara ait ogrenen bir sistem olabilir

	return totalPoint;
}

