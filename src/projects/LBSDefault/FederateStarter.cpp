#include "FederateStarter.h"

FederateStarter::FederateStarter()
{
}

FederateStarter::~FederateStarter(void)
{
	check = false;
}

void FederateStarter::StartFederateListener()
{
	TcpListener^ fedListener = gcnew TcpListener(8888); 
	fedListener->Start(); 
	check = true;

	Console::WriteLine("FedListener baslatildi..."); 

	Socket^ fedSocket = fedListener->AcceptSocket(); 
	SimAgent* simAgent;
	String^ message;
	while(true)
	{
		NetworkStream^ nStream = gcnew NetworkStream(fedSocket);

		try
		{
			// Five defined actions to perform the migration.
			// 1- Launches a new SimAgent  
			String^ fedType_Name = ReadString(nStream);
			cli::array<String^>^ fTNArray =  fedType_Name->Split('|');
			const string fedType = StringOperation::toStandardString(fTNArray[0]).c_str();
			const string fedName = StringOperation::toStandardString(fTNArray[1]).c_str();
			//simAgent = ::CreateFederate(fedType.c_str(),fedName.c_str());
			// Federate state is migrating!!!
			simAgent->GetLBSFederate()->SetFederateState(FEDERATESTATE::MIGRATING);

			String^ rstate = ReadString(nStream);
			if(rstate == "Saved")
			{
				//*Register federate to rti.
				//Request saved state.
				SendString("RequestState",nStream);
				//Restore saved state.
				cli::array<Byte>^ savedStateByte = ReadByteFromStream(nStream);
				//IMemArchive* memArch( plFactory->CrtMemArchive(AT_READ));
				//PersistenceLib::ArchiveConverter::ConvertManagedBuffertoArchive(savedStateByte,*memArch);
				//simAgent->FederateRestore(*memArch);
				delete savedStateByte;
				//Request messages.
				SendString("RequestMessages",nStream);
				//Restore messages.
				cli::array<Byte>^ savedMessagesByte = ReadByteFromStream(nStream);
				IMemArchive* memArch2( plFactory->CrtMemArchive(AT_READ));
				PersistenceLib::ArchiveConverter::ConvertManagedBuffertoArchive(savedMessagesByte,*memArch2);
				//simAgent->FederateRestore(*memArch2);
				delete savedMessagesByte;
				//Send success messages.
				SendString("Success",nStream);
			}

			nStream->Flush() ; 
		}
		catch(Exception ^ex)
		{
			Console::WriteLine("Sunucu kapatiliyor...");
			return ;
		}

	} //End While

	fedSocket->Close();
}
void FederateStarter::SendString(String^ message, NetworkStream^ networkStream)
{
	array<Byte>^ data = Encoding::Unicode->GetBytes(message);
	networkStream->Write(data, 0, data->Length);
	networkStream->Flush();
}

String^ FederateStarter::ReadString(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	nStream->Read(myTempBuffer, 0, 1024);
	String^ message = String::Concat(message, System::Text::Encoding::Unicode->GetString(myTempBuffer, 0, myTempBuffer->Length));
	return message;
}
cli::array<Byte>^ FederateStarter::ReadByteFromStream(NetworkStream^ nStream)
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

