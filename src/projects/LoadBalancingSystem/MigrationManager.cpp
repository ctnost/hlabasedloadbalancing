#include "StdAfx.h"
#include "MigrationManager.h"
#include "FederateStarter.h"

using namespace System;
using namespace System::IO;
using namespace System::Net::Sockets;
using namespace System::Threading;

MigrationManager::MigrationManager(void)
{
	FederateStarter^ fedStarter = gcnew FederateStarter();
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( fedStarter, &FederateStarter::StartFederateListener );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();
}

void MigrationManager::MigarateFederate(String^ ipAddress)
{
	TcpClient ^tcpClient;
	NetworkStream ^nStream;
	StreamReader ^sReader;
	StreamWriter ^sWriter;

	try
	{
		tcpClient = gcnew TcpClient(ipAddress, 1234);
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("Baglanamadi");
		return;
	}
	//Server programýnda yaptýklarýmýzý burda da yapýyoruz.
	nStream = tcpClient->GetStream();
	sWriter = gcnew StreamWriter(nStream);
	sReader = gcnew StreamReader(nStream);

}