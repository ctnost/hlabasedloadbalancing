#include "MigrationManager.h"
#include "StringOperation.h"
#include "StreamString.h"

//SimAgenState *SimAgenState::s_instance = 0;

using namespace System::Threading;
using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::IO::Pipes;

string ByteArrayToStr(array<Byte>^ byteArray);

MigrationManager::MigrationManager(SimAgent *agent)
{
	simAgent = agent;
	mmCanClose = false;
}

void MigrationManager::StartListener()
{
	// Anonymouse Pipe
	// Named Pipe
	//Read IP adress
	/*
	TcpClient ^tcpClient;
	NetworkStream ^nStream;

	try
	{
		tcpClient = gcnew TcpClient();
		tcpClient->Connect("127.0.0.1", 8888);
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("Baglanamadi");
		return;
	}
	nStream = tcpClient->GetStream();
	String^ ipAddress = ReadString(nStream);
	nStream->Close();
	tcpClient->Close();
	*/
	 String ^fedName =  StringOperation::toSystemString(simAgent->GetLBSFederate()->GetName());
	 Console::WriteLine("Wait For Pipe***" + fedName);
	 NamedPipeServerStream ^pipeServer = gcnew NamedPipeServerStream(fedName, PipeDirection::InOut, 1);
     // Wait for a client to connect
     pipeServer->WaitForConnection();
	  Console::WriteLine("*******************Wait For Pipe***" + fedName);
	 StreamString ^ss = gcnew StreamString(pipeServer);
	 String^ ipAddress = ss->ReadString();
	 pipeServer->Close();
	 MigarateFederate(ipAddress);
}
void MigrationManager::MigarateFederate(String^ ipAddress)
{
	TcpClient ^tcpClient;
	NetworkStream ^nStream;
	tcpClient = gcnew TcpClient(ipAddress, 8889);
	nStream = tcpClient->GetStream();
	
	LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
	dao->ConnectToDb();
	long migTime = dao->GetMigrationTime();
	long diff = time(0);
	diff = diff - migTime;

	const string fedType = simAgent->GetLBSFederate()->GetTypeName();
	const string fedName = simAgent->GetLBSFederate()->GetName();

	String^ fedType_Name = StringOperation::toSystemString(fedType) + "|" +  StringOperation::toSystemString(fedName) + "|" + diff.ToString();
	//Send FederateName
		WriteString(fedType_Name,nStream);
	// Federate state is SAVE!!!
		simAgent->GetLBSFederate()->SetFederateState(SAVE);
	//***
	nStream->Close();
	tcpClient->Close();
	bool isConnected = false;
	while(!isConnected){
		try{
			tcpClient = gcnew TcpClient(ipAddress, 8885);
			isConnected = true;
		}catch(Exception^ ex){
			Console::WriteLine("88885 Bekleniyor...");
			Thread::Sleep(100);
		}
	}
	nStream = tcpClient->GetStream();

	///***
	std::ofstream ofs("migratingFed");
	assert(ofs.good());
	boost::archive::binary_oarchive oa(ofs);
	simAgent->FederateSave(oa);
	ofs.close();
	
	cli::array<Byte>^ managedBuffer = System::IO::File::ReadAllBytes("migratingFed");

	//Send Saved State
		WriteString("Saved",nStream);
	//Read State
	String^ rstate = ReadString(nStream);
	if(CheckFedState(rstate,"RequestState"))
		nStream->Write( managedBuffer, 0, managedBuffer->Length );

	//Read State
	rstate = ReadString(nStream);
	if(CheckFedState(rstate,"RequestMessages"))
	{
		std::ofstream ofs2("migratingReq");
		assert(ofs2.good());
		boost::archive::binary_oarchive oa2(ofs2);
		simAgent->RequestInteractions(oa2);
		//Kesildi
		//Close SimAgent
		SimAgenState::getInstance()->set_value(2);
		//
		ofs2.close();
		cli::array<Byte>^ managedBuffer2 = System::IO::File::ReadAllBytes("migratingReq");
		nStream->Write( managedBuffer2, 0, managedBuffer2->Length );	
	}

	//rstate = ReadString(nStream);
	mmCanClose = false;
	//if(CheckFedState(rstate,"Success"))
	//	SimAgenState::getInstance()->set_value(2);
	
	//rstate = ReadString(nStream);
	//if(CheckFedState(rstate,"ReRequestMessages"))
	//{
	//	std::ofstream ofs2("migratingReReq");
	//	assert(ofs2.good());
	//	boost::archive::binary_oarchive oa2(ofs2);
	//	simAgent->RequestInteractions(oa2);
	//	ofs2.close();
	//	cli::array<Byte>^ managedBuffer2 = System::IO::File::ReadAllBytes("migratingReReq");
	//	nStream->Write( managedBuffer2, 0, managedBuffer2->Length );	
	//}
	mmCanClose = true;
	nStream->Close();
	tcpClient->Close();
}

bool MigrationManager::CheckFedState(String^ fedState, String^ controlState)
{
	const string constFedState = StringOperation::toStandardString(fedState).c_str();
	const string constControlState = StringOperation::toStandardString(controlState).c_str();
	if(strcmp(constFedState.c_str(),constControlState.c_str()) == 0 )
	{
		return true;
	}
	return false;
}

void MigrationManager::WriteString(String^ message, NetworkStream^ networkStream)
{
	array<Byte>^ data = System::Text::Encoding::Unicode->GetBytes(message);
	networkStream->Write(data, 0, data->Length);
	networkStream->Flush();
}

String^ MigrationManager::ReadString(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	nStream->Read(myTempBuffer, 0, 1024);
	String^ message = String::Concat(message, System::Text::Encoding::Unicode->GetString(myTempBuffer, 0, myTempBuffer->Length));
	message = message->Replace("\0",String::Empty);
	return message;
}
cli::array<Byte>^ MigrationManager::ReadByteFromStream(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	cli::array<Byte>^ myReadBuffer = gcnew array<Byte>(0);
	String^ myCompleteMessage = "";
	int numberOfBytesRead = 0;
	int sumOfBytesRead = 0;

	// Incoming message may be larger than the buffer size.
	do
	{
		numberOfBytesRead = nStream->Read( myReadBuffer, 0, myReadBuffer->Length );
		Array::Resize(myReadBuffer,myReadBuffer->Length + numberOfBytesRead);
		Array::Copy(myTempBuffer,0,myReadBuffer,myReadBuffer->Length,numberOfBytesRead);
		sumOfBytesRead += numberOfBytesRead;
	}
	while ( nStream->DataAvailable );

	return myReadBuffer;
}


string ByteArrayToStr(array<Byte>^ byteArray)
{
	int size = byteArray->Length;
	IntPtr pnt = Marshal::AllocHGlobal(size + 1);
	char* chararray = (char*)pnt.ToPointer();
	try
	{
		Marshal::Copy(byteArray, 0, pnt, size);
		*(chararray + size) = 0;
		return string(chararray);
	}
	finally
	{
		Marshal::FreeHGlobal(pnt);
	}
}