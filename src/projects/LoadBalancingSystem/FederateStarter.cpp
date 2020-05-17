#include "StdAfx.h"
#include "FederateStarter.h"
#include "SimAgent.h"
#include "RTIAmbassadorInstance.h"

using namespace System;
using namespace System::IO;
using namespace System::Net::Sockets;

FederateStarter::FederateStarter(void)
{
}

FederateStarter::~FederateStarter(void)
{
	check = false;
}

void FederateStarter::StartFederateListener()
{
	TcpListener^ fedListener = gcnew TcpListener(1234); 
	fedListener->Start(); 
	check = true;

	Console::WriteLine("FedListener baslatildi..."); 

	Socket^ fedSocket = fedListener->AcceptSocket(); 
	SimAgent* simAgent;
	while(true)
	{
		NetworkStream^ nStream = gcnew NetworkStream(fedSocket);
		StreamWriter^ sWriter = gcnew StreamWriter(nStream);
		StreamReader^ sReader = gcnew StreamReader(nStream);

		try
		{
			// Five defined actions to perform the migration.
			// 1- Launches a new SimAgent  
			String^ fedName = sReader->ReadLine();
			LBSDefaultFederate* lbsDefFed ;//= new LBSDefaultFederate();
			lbsDefFed->Init(RTIAmbassadorInstance::GetInstance());
			simAgent = new SimAgent(lbsDefFed);
			

			Console::WriteLine("Gelen Bilgi:" + fedName);
			sWriter->WriteLine("c");
			sWriter->Flush() ; 
		}
		catch(Exception ^ex)
		{
			Console::WriteLine("Sunucu kapatiliyor...");
			return ;
		}

	} //End While

	fedSocket->Close();
}