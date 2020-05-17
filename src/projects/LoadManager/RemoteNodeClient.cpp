#include "StdAfx.h"
#include "RemoteNodeClient.h"
#include "StreamString.h"
#include "Utility.h"

using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::IO::Pipes;
using namespace System::Net::Sockets;

RemoteNodeClient::RemoteNodeClient(void)
{
}

int RemoteNodeClient::AskForMigration(String^ ipAddress)
{
	try{
		TcpClient ^tcpClient = gcnew TcpClient(ipAddress, 8881);
		NetworkStream ^nStream = tcpClient->GetStream();
		Utility::WriteString("AvailableForMigration",nStream);
		String^ mState = Utility::ReadString(nStream);
		return Convert::ToInt32(mState);
	}catch(Exception^ ex){
		return -5; 
	}
}

int RemoteNodeClient::HasAnyMigrationRequest(String^ ipAddress)
{
	TcpClient ^tcpClient = gcnew TcpClient(ipAddress, 8881);
	NetworkStream ^nStream = tcpClient->GetStream();
	Utility::WriteString("MigrationRequest",nStream);
	String^ mState = Utility::ReadString(nStream);
	return Convert::ToInt32(mState);	
}

String^ RemoteNodeClient::GetUtilization(String^ ipAddress)
{
	try{
		TcpClient ^tcpClient = gcnew TcpClient(ipAddress, 8881);
		NetworkStream ^nStream = tcpClient->GetStream();
		Utility::WriteString("GetUtilization",nStream);
		String^ mState = Utility::ReadString(nStream);
		return mState;
	}catch(Exception^ ex){
	}
	return "";
}

String^ RemoteNodeClient::GetFederates(String^ ipAddress)
{
	try{
		TcpClient ^tcpClient = gcnew TcpClient(ipAddress, 8881);
		NetworkStream ^nStream = tcpClient->GetStream();
		Utility::WriteString("GetFederates",nStream);
		String^ mState = Utility::ReadString(nStream);
		return mState;
	}catch(Exception^ ex){
	}
	return "";
}

void RemoteNodeClient::SetMigrationStartTime(String^ ipAddress,long migStartTime)
{
	try{
		TcpClient ^tcpClient = gcnew TcpClient(ipAddress, 8881);
		NetworkStream ^nStream = tcpClient->GetStream();
		Utility::WriteString("StartTime|"+migStartTime.ToString(),nStream);
	}catch(Exception^ ex){
	}
}