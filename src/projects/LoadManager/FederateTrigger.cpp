#include "FederateTrigger.h"
#include "CommonHeaders.h"
#include "StreamString.h"

using namespace System::IO::Pipes;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Principal;

FederateTrigger::FederateTrigger()
{
}

FederateTrigger::~FederateTrigger(void)
{
}

void FederateTrigger::StartFederateTrigger()
{
	TcpListener^ fedListener = gcnew TcpListener(8887); 
	fedListener->Start(); 

	Console::WriteLine("FedTrigger baslatildi..."); 
 
	while(true)
	{
		Socket^ fedSocket = fedListener->AcceptSocket();
		Console::WriteLine("FedTrigger acceptsocket..."); 
		NetworkStream^ nStream = gcnew NetworkStream(fedSocket);
		String^ readedString = ReadString(nStream);
		cli::array<String^>^ rArray =  readedString->Split('|');
		String^ ipAddress = rArray[0];
		String^ fedName = rArray[1]->Replace("\0",String::Empty);
		//Trigger NamedPipe
		NamedPipeClientStream^ pipeClient = gcnew NamedPipeClientStream
			(".",fedName,PipeDirection::InOut, PipeOptions::None, TokenImpersonationLevel::Impersonation);
		pipeClient->Connect();
		Console::WriteLine("FedTrigger PipeClientConnect..."); 
		StreamString^ ss = gcnew StreamString(pipeClient);
		ss->WriteString(ipAddress);
		pipeClient->Close();
		//
		nStream->Flush();

		fedSocket->Close();
		Console::WriteLine("FedTrigger Close..."); 
	} //End While

}

void FederateTrigger::SendString(String^ message, NetworkStream^ networkStream)
{
	array<Byte>^ data = Encoding::Unicode->GetBytes(message);
	networkStream->Write(data, 0, data->Length);
	networkStream->Flush();
}

String^ FederateTrigger::ReadString(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	nStream->Read(myTempBuffer, 0, 1024);
	String^ message = String::Concat(message, System::Text::Encoding::Unicode->GetString(myTempBuffer, 0, myTempBuffer->Length));
	return message;
}
cli::array<Byte>^ FederateTrigger::ReadByteFromStream(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	cli::array<Byte>^ myReadBuffer = gcnew array<Byte>(0);
	String^ myCompleteMessage = "";
	int numberOfBytesRead = 0;
	int sumOfBytesRead = 0;

	// Incoming message may be larger than the buffer size.
	do
	{
		numberOfBytesRead = nStream->Read( myTempBuffer, 0, myTempBuffer->Length );
		Array::Resize(myReadBuffer,myReadBuffer->Length + numberOfBytesRead);
		Array::Copy(myTempBuffer,0,myReadBuffer,0,numberOfBytesRead);
		sumOfBytesRead += numberOfBytesRead;
	}
	while ( nStream->DataAvailable );

	return myReadBuffer;
}